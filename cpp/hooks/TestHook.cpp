#include "headers/pch.h"

#include "headers/utils/RegistersUtils.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/HookData.h"
#include "headers/hooks/TestHook.h"
#include "headers/utils/CodingUtils.h"

using namespace HooksNS::TestHook;


void TestHookedCode() {
    ADDRESS_TYPE rax;
    ADDRESS_TYPE rcx;
    ADDRESS_TYPE rdx;
    ADDRESS_TYPE rbx;
    ADDRESS_TYPE rsp;
    ADDRESS_TYPE rbp;
    ADDRESS_TYPE rsi;
    ADDRESS_TYPE rdi;
    ADDRESS_TYPE r8;
    ADDRESS_TYPE r9;
    ADDRESS_TYPE r10;
    ADDRESS_TYPE r11;
    ADDRESS_TYPE r12;
    ADDRESS_TYPE r13;
    ADDRESS_TYPE r14;
    ADDRESS_TYPE r15;

    asm("mov %%rax, %0" : "=r" (rax));
    asm("mov %%rcx, %0" : "=r" (rcx));
    asm("mov %%rdx, %0" : "=r" (rdx));
    asm("mov %%rbx, %0" : "=r" (rbx));
    asm("mov %%rsp, %0" : "=r" (rsp));
    asm("mov %%rbp, %0" : "=r" (rbp));
    asm("mov %%rsi, %0" : "=r" (rsi));
    asm("mov %%rdi, %0" : "=r" (rdi));
    asm("mov %%r8, %0" : "=r" (r8));
    asm("mov %%r9, %0" : "=r" (r9));
    asm("mov %%r10, %0" : "=r" (r10));
    asm("mov %%r11, %0" : "=r" (r11));
    asm("mov %%r12, %0" : "=r" (r12));
    asm("mov %%r13, %0" : "=r" (r13));
    asm("mov %%r14, %0" : "=r" (r14));
    asm("mov %%r15, %0" : "=r" (r15));

    std::cout << rax << std::endl;
}


Data::Data() {
    this->SetModuleName(L"EngineWin64s.dll");

    this->SetBytesToReplace({
        0x48, 0x89, 0x07, // mov [rdi], rax
        0x41, 0x8B, 0x44, 0xD1, 0x18, // mov eax, [r9+rdx*8+18]
        0x89, 0x47, 0x08, // mov [rdi+08], eax
        0x48, 0x8B, 0x4E, 0x20, // mov rcx,[rsi+20]
        0x48, 0x8B, 0x04, 0xD1 // mov rax,[rcx+rdx*8]  <---- Here is where the item address is
    });

    this->SetBytesToReplaceAddressOffset("0x383585");
}

void Data::PrepareTrampolineBytes(std::vector<BYTE> trampolineSkeleton, UINT jmpSkeletonSize) {
    if (this->architecture != 0x86 && this->architecture != 0x64) {
        throw std::exception("Invalid architecture");
    }

    std::vector<BYTE> trampoline = this->GetBytesToReplace();
    trampoline.insert(trampoline.end(), trampolineSkeleton.begin(), trampolineSkeleton.end());
        
    UINT index;
    if (this->architecture == 0x86) {
        index = this->GetBytesToReplace().size() + 2;
    } else {
        index = this->GetBytesToReplace().size() + 33;
    }

    std::vector<BYTE> functionAddressVector = CodingUtils::ToReversedBytesVector((ADDRESS_TYPE)TestHookedCode);
    std::vector<BYTE> originalAddressVector = CodingUtils::ToReversedBytesVector(
        (ADDRESS_TYPE)this->GetOriginalAddress() + (jmpSkeletonSize - 2)
    );

    RegistersUtils::Register registerToUse = this->GetRegisterForSafeJump();

    std::vector<BYTE> movRegisterBytes = RegistersUtils::MOVInstructionBytes(registerToUse);
    CodingUtils::ByteArrayReplace(index, &movRegisterBytes, &trampoline);
    index = index + movRegisterBytes.size();
    CodingUtils::ByteArrayReplace(index, &functionAddressVector, &trampoline);

    index = index + sizeof(ADDRESS_TYPE);
    std::vector<BYTE> callRegisterBytes = RegistersUtils::CALLInstructionBytes(registerToUse);
    CodingUtils::ByteArrayReplace(index, &callRegisterBytes, &trampoline);

    index = index + callRegisterBytes.size();
    if (this->architecture == 0x86) {
        index = index + 2;
    } else {
        index = index + 33;
    }
    CodingUtils::ByteArrayReplace(index, &movRegisterBytes, &trampoline);
    index = index + movRegisterBytes.size();
    CodingUtils::ByteArrayReplace(index, &originalAddressVector, &trampoline);
    index = index + sizeof(ADDRESS_TYPE);

    std::vector<BYTE> jmpRegisterBytes = RegistersUtils::JMPInstructionBytes(registerToUse);
    CodingUtils::ByteArrayReplace(index, &jmpRegisterBytes, &trampoline);

    this->trampolineBytes = trampoline;
}

void Data::InitFeatures() {
    Features::TEST_FEATURE = (FeaturesNS::TestFeatureNS::Feature*)this->featuresHandler->GetFeature(FeaturesNS::TestFeatureNS::NAME);
}

BOOL Data::DirectReadWithOffset() {
    return TRUE;
}

std::vector<BYTE>* Data::GetTrampolineBytes(UINT jmpSkeletonSize) {
    HOOK_ADDRESS = (ADDRESS_TYPE)TestHookedCode;

    // - 2 because the last 2 bytes of the JMP are the POP instruction that we need to execute.
    RETURN_ADDRESS = (ADDRESS_TYPE)this->GetOriginalAddress() + (jmpSkeletonSize - 2);

    return &this->trampolineBytes;
}

RegistersUtils::Register Data::GetRegisterForSafeJump() {
    return RegistersUtils::Register::R13;
}