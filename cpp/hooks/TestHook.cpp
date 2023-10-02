#include "headers/pch.h"

#include "headers/utils/RegistersUtils.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/HookData.h"
#include "headers/hooks/TestHook.h"
#include "headers/utils/CodingUtils.h"

using namespace HooksNS::TestHook;


void TestHookedCode() {
    ADDRESS_TYPE rax = 0x0;
    ADDRESS_TYPE rcx = 0x0;
    ADDRESS_TYPE rdx = 0x0;
    ADDRESS_TYPE rbx = 0x0;
    ADDRESS_TYPE rsp = 0x0;
    ADDRESS_TYPE rbp = 0x0;
    ADDRESS_TYPE rsi = 0x0;
    ADDRESS_TYPE rdi = 0x0;
    ADDRESS_TYPE r8 = 0x0;
    ADDRESS_TYPE r9 = 0x0;
    ADDRESS_TYPE r10 = 0x0;
    ADDRESS_TYPE r11 = 0x0;
    ADDRESS_TYPE r12 = 0x0;
    ADDRESS_TYPE r13 = 0x0;
    ADDRESS_TYPE r14 = 0x0;
    ADDRESS_TYPE r15 = 0x0;

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

ADDRESS_TYPE Data::GetHookFunctionAddress() {
    return (ADDRESS_TYPE)TestHookedCode;
}

HookerNS::OriginalBytesBehaviour Data::GetOriginalBytesBehaviour() {
    return HookerNS::OriginalBytesBehaviour::BeforeFunctionCall;
}

void Data::InitFeatures() {
    Features::TEST_FEATURE = (FeaturesNS::TestFeatureNS::Feature*)this->featuresHandler->GetFeature(FeaturesNS::TestFeatureNS::NAME);
}

BOOL Data::DirectReadWithOffset() {
    return TRUE;
}

RegistersUtils::Register Data::GetRegisterForSafeJump() {
    return RegistersUtils::Register::R13;
}