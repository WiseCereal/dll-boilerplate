#include "headers/pch.h"

#include "headers/utils/RegistersUtils.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/HookData.h"
#include "headers/hooks/TestHook.h"

using namespace HooksNS::TestHook;

/*
void __declspec(naked) TestHookTrampoline() {
    __asm {
        // Push registers and flags
        pushfd
        pushad

        // Call hook
        call[HOOK_ADDRESS]

        // Restore registers and flags
        popad
        popfd

        // JMP back to the original code
        jmp[RETURN_ADDRESS]
    }
}
*/

void TestHookTrampoline() {

}

void TestHookedCode(RegistersUtils::x64Registers registers, ADDRESS_TYPE flags) {

}


Data::Data() {
    /* 
    // HADES
    this->SetBytesToReplace({
        0x48, 0x8B, 0x4E, 0x20, //  mov rcx,[rsi+20]
        0x48, 0x8B, 0x04, 0xD1, // mov rax,[rcx+rdx*8]  <---- Here is where the item address is
        0x48, 0x89, 0x47, 0x10, //  mov [rdi+10],rax
        0x8B, 0x44, 0xD1, 0x08, // mov eax,[rcx + rdx * 8 + 08]
    });

    this->SetBytesToReplaceAddressOffset("0x383590");
    */
    this->SetBytesToReplace({
        0x41, 0x83, 0xE3, 0xF8,
        0x4C, 0x8B, 0xC9,
        0x41, 0xF6, 0x00, 0x04,
        0x4C, 0x8B, 0xD1
    });

    this->SetBytesToReplaceAddressOffset("0x82DB0");


    // Looks like trampoline bytes are identical except for the register
    // and the address.
    // So save the skeleton of trampoline bytes in HookData
    // and just overwrite them here with the correct values.
    /*
        PUSH ALL REGISTERS // in x64 looks like I have to manually push each one with push rax, push rsi, push r9, etc etc etc
        PUSH FLAGS

        mov rax,0x7fffffffffff // Move the ABSOLUTE address of our DLL c++ function
        CALL rax // Call our dll c++ function. THIS MUST BE A NAKED FUNCTION.

        POP FLAGS
        POP ALL REGISTERS

        MOV rax,0x7fffffffffff // Return address right after we set up the jump to this trampoline (the POP instruction)
        JMP rax
    */
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

std::vector<BYTE> Data::GetTrampolineBytes(UINT jmpSkeletonSize) {
    HOOK_ADDRESS = (ADDRESS_TYPE)TestHookedCode;
    RETURN_ADDRESS = (ADDRESS_TYPE)this->GetOriginalAddress() + jmpSkeletonSize;

    return this->trampolineBytes;
}

RegistersUtils::Register Data::GetRegisterForSafeJump() {
    return RegistersUtils::Register::RAX;
}