#include "headers/pch.h"

#include "headers/services/FeaturesHandler/Feature.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/Hooker.h"
#include "headers/services/Hooker/TrampolineSkeletons.h"
#include "headers/exceptions/NotFoundException.h"
#include "headers/utils/CodingUtils.h"
#include "headers/utils/RegistersUtils.h"

using namespace HookerNS;

Service::Service(
    UINT architecture,
    FeaturesHandlerNS::Service* featuresHandler
) {
    this->architecture = architecture;
    this->featuresHandler = featuresHandler;

    this->initJmpSkeleton();

    this->hooksVector.push_back(&this->testHook);

    this->validateHooks();
}

Service* Service::Reset() {
    for (auto hook : this->hooksVector) {
        this->_disableHook(hook);
        hook->Reset();
        hook->SetRequestToEndThread();
    }

    while (this->AreThereAnyInitAddressThreadsRunning()) {
        Sleep(20);
    }

    return this;
}

std::vector<HookerNS::HookData*> Service::GetHooksVector() {
    return this->hooksVector;
}

Service* Service::InitHooks() {
    this->initHookThreads.clear();
    HANDLE t;
    for (auto hook : this->hooksVector) {
        hook->PrepareTrampolineBytes(this->getTrampolineSkeleton());

        t = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Threads::InitAddressesThread, hook, 0, NULL);
        if (t) {
            this->initHookThreads.push_back(t);
        }
        else {
            throw std::runtime_error("Error while creating the thread for the hook " + hook->GetName());
        }
        t = NULL;
    }

    t = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Threads::OnInitAddressesThreadFinish, this, 0, NULL);
    if (t) {
        WaitForSingleObject(t, INFINITE);
        CloseHandle(t);
    }
    else {
        throw std::runtime_error("Error while creating the thread that waits for the hook init threads to finish ");
    }

    return this;
}

BOOL Service::AreThereAnyInitAddressThreadsRunning() {
    for (auto hook : this->hooksVector) {
        if (hook->GetIsFindOriginalAddressThreadRunning()) {
            return TRUE;
        }
    }

    return FALSE;
}

Service* Service::EnableHook(std::string hookName) {
    this->_enableHook(this->findHook(hookName));
    return this;
}

Service* Service::DisableHook(std::string hookName) {
    this->_disableHook(this->findHook(hookName));
    return this;
}

Service* Service::EnableAllHooks() {
    for (auto hook : this->hooksVector) {
        if (hook->ShouldEndThread()) {
            continue;
        }
        this->_enableHook(hook);
    }
    return this;
}

Service* Service::DisableAllHooks() {
    for (auto hook : this->hooksVector) {
        this->_disableHook(hook);
    }
    return this;
}


std::vector<HANDLE> Service::GetInitHookThreads() {
    return this->initHookThreads;
}

FeaturesHandlerNS::Service* Service::GetFeaturesHandler() {
    return this->featuresHandler;
}

Service* Service::validateHooks() {
    std::vector<std::string> hooksNames;
    for (auto hook : this->hooksVector) {
        if (!hook->GetBytesToReplaceAddressOffset()) {
            throw std::runtime_error("Hook " + hook->GetName() + " doesn't have an address offset.");
        }
        if (hook->GetBytesToReplace().size() < this->jmpSkeleton.size()) {
            throw std::runtime_error("Hook " + hook->GetName() + " doesn't have enough bytes to replace.");
        }

        for (auto& hookName : hooksNames) {
            if (hookName == hook->GetName()) {
                throw std::runtime_error("Duplicated hook: " + hookName);
            }
        }
        hooksNames.push_back(hook->GetName());
    }

    return this;
}

HookerNS::HookData* Service::findHook(std::string hookName) {
    for (auto hook : this->hooksVector) {
        if (hook->GetName() == hookName) {
            return hook;
        }
    }

    CustomExceptionsNS::NotFoundException ex;
    ex.setWhat("Hook " + hookName + " doesn't exist.");
    throw ex;
}


Service* Service::_enableHook(HookerNS::HookData* hookData) {
    if (!hookData->GetOriginalAddress()) {
        throw std::runtime_error("Can't enable hook " + hookData->GetName() + " - originalAddress not set");
    }

    size_t len = hookData->GetBytesToReplace().size();
    LPVOID originalAddress = hookData->GetOriginalAddress();

    DWORD originalProtection;
    VirtualProtect(originalAddress, len, PAGE_EXECUTE_READWRITE, &originalProtection);

    memset(originalAddress, 0x90, len);

    std::vector<BYTE>* trampolineBytes = hookData->GetTrampolineBytes((UINT)this->jmpSkeleton.size());
    ADDRESS_TYPE trampolineBytesAddress = (ADDRESS_TYPE)trampolineBytes->data();

    std::vector<BYTE> jmpToTrampolineBytes = this->jmpSkeleton;

    RegistersUtils::Register registerToUse = hookData->GetRegisterForSafeJump();
    std::vector<BYTE> movRegisterBytes = RegistersUtils::MOVInstructionBytes(registerToUse);
    std::vector<BYTE> jmpRegisterBytes = RegistersUtils::JMPInstructionBytes(registerToUse);
    std::vector<BYTE> pushRegisterBytes = RegistersUtils::PUSHInstructionBytes(registerToUse);
    std::vector<BYTE> popRegisterBytes = RegistersUtils::POPInstructionBytes(registerToUse);

    std::vector<BYTE> trampolineBytesAddressVector = CodingUtils::ToReversedBytesVector(trampolineBytesAddress);

    UINT startIndexOfMovInstruction = 2;

    CodingUtils::ByteArrayReplace(0, &pushRegisterBytes, &jmpToTrampolineBytes);
    CodingUtils::ByteArrayReplace(startIndexOfMovInstruction, &movRegisterBytes, &jmpToTrampolineBytes);
    CodingUtils::ByteArrayReplace(startIndexOfMovInstruction + movRegisterBytes.size(), &trampolineBytesAddressVector, &jmpToTrampolineBytes);
    CodingUtils::ByteArrayReplace(jmpToTrampolineBytes.size() - 5, &jmpRegisterBytes, &jmpToTrampolineBytes);
    CodingUtils::ByteArrayReplace(jmpToTrampolineBytes.size() - 2, &popRegisterBytes, &jmpToTrampolineBytes);

    for (size_t i = 0; i < jmpToTrampolineBytes.size(); i++) {
        ADDRESS_TYPE addr = (ADDRESS_TYPE)originalAddress + i;
        memset((LPVOID)addr, jmpToTrampolineBytes.at(i), 1);
    }

    DWORD _;
    VirtualProtect((LPVOID)trampolineBytesAddress, trampolineBytes->size(), PAGE_EXECUTE_READWRITE, &_);
    VirtualProtect(originalAddress, len, originalProtection, &_);

    return this;
}

Service* Service::_disableHook(HookerNS::HookData* hookData) {
    if (!hookData->GetOriginalAddress()) {
        return this;
    }

    auto originalBytes = hookData->GetBytesToReplace();
    auto len = originalBytes.size();
    auto originalAddress = hookData->GetOriginalAddress();

    DWORD originalProtection;
    VirtualProtect(originalAddress, len, PAGE_EXECUTE_READWRITE, &originalProtection);

    for (size_t i = 0; i < len; i++) {
        ADDRESS_TYPE addr = (ADDRESS_TYPE)originalAddress + i;
        memset((LPVOID)addr, originalBytes.at(i), 1);
    }

    DWORD _;
    VirtualProtect(originalAddress, len, originalProtection, &_);

    return this;
}

Service* Service::initJmpSkeleton() {
    switch (this->architecture) {
    case 0x86:
        this->jmpSkeleton = {
            0x00, 0x00, // push $register
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov $register, $address
            0x00, 0x00, 0x00, // jmp $register,
            0x00, 0x00 // pop $register
        };
        break;
    case 0x64:
        this->jmpSkeleton = {
            0x00, 0x00, // push $register
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov $register, $address
            0x00, 0x00, 0x00, // jmp $register
            0x00, 0x00 // pop $register
        };
        break;

    default:
        throw std::exception("Invalid architecture.");
        break;
    }

    return this;
}

std::vector<BYTE> Service::getTrampolineSkeleton() {
    switch (this->architecture) {
    case 0x86:
        return TrampolineSkeletons::x86;
    case 0x64:
        return TrampolineSkeletons::x64;
    default:
        throw std::exception("Invalid architecture");
    }
}