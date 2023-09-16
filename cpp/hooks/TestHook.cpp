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
    this->SetBytesToReplace({
        0x48, // dec eax
        0x89, 0x85, 0x10, 0xFF, 0xFF, 0xFF, // mov [ebp-000000F0], eax
        0x89, 0x42, 0x58 // mov [edx+58], eax  <--- that's the one that writes the memory
        });

    this->SetBytesToReplaceAddressOffset("0x7A11");
}

void Data::InitFeatures() {
    Features::TEST_FEATURE = (FeaturesNS::TestFeatureNS::Feature*)this->featuresHandler->GetFeature(FeaturesNS::TestFeatureNS::NAME);
}

BOOL Data::DirectReadWithOffset() {
    return TRUE;
}

ADDRESS_TYPE Data::GetTrampolineRef() {
    HOOK_ADDRESS = (ADDRESS_TYPE)TestHookedCode;
    RETURN_ADDRESS = (ADDRESS_TYPE)this->GetOriginalAddress() + 5;
    return (ADDRESS_TYPE)TestHookTrampoline;
}

RegistersUtils::Register Data::GetRegisterForSafeJump() {
    return RegistersUtils::Register::RAX;
}