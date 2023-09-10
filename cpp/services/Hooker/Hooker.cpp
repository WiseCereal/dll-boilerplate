#include "headers/pch.h"

#include "headers/services/FeaturesHandler/Feature.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/Hooker.h"
#include "headers/exceptions/NotFoundException.h"

using namespace HookerNS;

Service::Service(FeaturesHandlerNS::Service* featuresHandler) {
    this->featuresHandler = featuresHandler;

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
        hook->SetScanStartingAddress(GetModuleHandle(NULL)); // TODO: starting address can be different for each hook
        // Properly identify the module for the hook and set the base address of that module here.

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
        if (hook->GetBytesToReplace().size() < 5) {
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

    auto len = hookData->GetBytesToReplace().size();
    auto originalAddress = hookData->GetOriginalAddress();

    // Change the protection of the opcode address so we can overwrite it.
    DWORD originalProtection;
    VirtualProtect(originalAddress, len, PAGE_EXECUTE_READWRITE, &originalProtection);

    // The bytesToReplace are replaced by NOPs
    memset(originalAddress, 0x90, len);

    // Calculate the offset of trampoline. The DLL functions will always be at higher memory addresses than the original code.
    // So we just subtract the address of trampoline with the address of originalAddress and this will give us the offset.
    // Then, we subtract the JMP opcode size to this offset.

    // Example: Let's say trampoline starts at address 0x666, and the originalAddress is at address 0x555.
    // By subtracting them, we get the offset: 0x666 - 0x555 = 0x111
    // Since the JMP instruction is 5 bytes, we also need to subtract the JMP instruction from this offset: 0x111 - 0x5 = 0x10C
    // So now we have the correct offset of trampoline! :) The assembly instruction JMP 0x10C means, transfer the flow of the code to the address
    // starting at 0x10C from here
    ADDRESS_TYPE trampolineRef = hookData->GetTrampolineRef();
    ADDRESS_TYPE offset = (trampolineRef - (ADDRESS_TYPE)originalAddress) - 5;


    // Now we replace the bytes at originalAddress with the "JMP offset" instruction bytes.
    // The remaining bytes of bytesToFind are NOPs, as we did that in the previous step.
    // So we are replacing the first 5 NOPs with the JMP instruction.

    // Casting the originalAddress to a BYTE pointer. This way, we replace the first NOP with the 0xE9 byte, the start of the relative JMP instruction.
    *(BYTE*)originalAddress = 0xE9;

    // Then, we add the offset
    *(ADDRESS_TYPE*)((ADDRESS_TYPE)originalAddress + 1) = offset;

    // And we are done! The bytes have been replaced with the JMP instruction. Let's restore the original protection.
    DWORD p;
    VirtualProtect(originalAddress, len, originalProtection, &p);

    return this;
}

Service* Service::_disableHook(HookerNS::HookData* hookData) {
    if (!hookData->GetOriginalAddress()) {
        return this;
        // throw std::runtime_error("Can't disable hook " + hookData->GetName() + " - originalAddress not set");
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

    DWORD p;
    VirtualProtect(originalAddress, len, originalProtection, &p);

    return this;
}
