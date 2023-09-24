#include "headers/pch.h"

#include "headers/utils/RegistersUtils.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/HookData.h"
#include "headers/hooks/TestHook.h"

using namespace HooksNS::TestHook;


void TestHookedCode(RegistersUtils::x64Registers registers, ADDRESS_TYPE flags) {
    // This is the function that should be marked as naked, but try first without the naked thing
    // and see if it works.
    std::cout << "It works!" << std::endl;
}


Data::Data() {
    this->SetModuleName(L"EngineWin64s.dll");

    this->SetBytesToReplace({
        0x89, 0x47, 0x08, // mov [rdi+08], eax
        0x48, 0x8B, 0x4E, 0x20, // mov rcx,[rsi+20]
        0x48, 0x8B, 0x04, 0xD1, // mov rax,[rcx+rdx*8]  <---- Here is where the item address is
        0x48, 0x89, 0x47, 0x10, // mov [rdi+10],rax
        0x8B, 0x44, 0xD1, 0x08, // mov eax,[rcx + rdx * 8 + 08]
    });

    this->SetBytesToReplaceAddressOffset("0x38358D");

    // prepare trampoline bytes using x32 or x64 jmp skeleton and then set them
    this->SetTrampolineBytes({
        0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
    });
}

void Data::InitFeatures() {
    Features::TEST_FEATURE = (FeaturesNS::TestFeatureNS::Feature*)this->featuresHandler->GetFeature(FeaturesNS::TestFeatureNS::NAME);
}

BOOL Data::DirectReadWithOffset() {
    return TRUE;
}

std::vector<BYTE>* Data::GetTrampolineBytes(UINT jmpSkeletonSize) {
    HOOK_ADDRESS = (ADDRESS_TYPE)TestHookedCode;
    RETURN_ADDRESS = (ADDRESS_TYPE)this->GetOriginalAddress() + jmpSkeletonSize;

    return &this->trampolineBytes;
}

RegistersUtils::Register Data::GetRegisterForSafeJump() {
    return RegistersUtils::Register::R13;
}