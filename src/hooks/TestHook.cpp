#include "pch.h"

#include "src/utils/RegistersUtils.h"
#include "src/services/Features/Service.h"
#include "src/services/Hooker/HookData.h"
#include "src/hooks/TestHook.h"
#include "src/utils/CodingUtils.h"

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

    //TODO: USE ReadProcessMemory to read memory where you are unsure if it's good memory and that's it :)
    // and remove min and max address, no need for this anymore if you use the power of ReadProcessMemory 


    ADDRESS_TYPE minAddress = 0x10000000000;
    ADDRESS_TYPE maxAddress = 0x7FFFFFFFFFFF;

    ADDRESS_TYPE itemTypeStructAddress = rax + 0x10;

    if (itemTypeStructAddress < minAddress || itemTypeStructAddress > maxAddress) {
        return;
    }


    ADDRESS_TYPE checkAddress = rax + 0x08;

    UINT checkValue = *(UINT*)checkAddress;
    if (checkValue == 0) {
        return;
    }

    auto itemTypeStruct = ((ADDRESS_TYPE)(*(LPVOID*)itemTypeStructAddress));
    auto itemTypeStringAddress = itemTypeStruct + 0x18;

    if (itemTypeStringAddress < minAddress || itemTypeStringAddress > maxAddress) {
        return;
    }

    std::vector<BYTE> str = {};
    size_t i = 0;
    while (true) {
        BYTE b = *(BYTE*)(itemTypeStringAddress + i);

        if (b == 0x00) {
            break;
        }
        if (i > 16) {
            break;
        }

        str.push_back(b);
        i++;
    }

    std::string holyGrail(str.begin(), str.end());

    if (holyGrail == "SuperLockKeys") {
        auto v = (*(double*)rax);
        std::cout << "titan blood -> " << v << std::endl;
        std::cout << " ------------- " << std::endl;
    }
}


Data::Data() {
    this->SetModuleName(L"EngineWin64s.dll");

    this->SetBytesToReplace({
        0x48, 0x23, 0xC8, // and rcx,rax
        0x49, 0x8B, 0x40, 0x20, // mov rax,[r8+20]
        0x48, 0x8D, 0x0C, 0x89, // lea rcx,[rcx+rcx*4]
        0x48, 0x8D, 0x04, 0xC8, // lea rax,[rax+rcx*8]
        0x0F, 0x1F, 0x00 // nop dword ptr [rax]
        });

    this->SetBytesToReplaceAddressOffset("0x382EFE");
}

ADDRESS_TYPE Data::GetHookFunctionAddress() {
    return (ADDRESS_TYPE)TestHookedCode;
}

HookerNS::OriginalBytesBehaviour Data::GetOriginalBytesBehaviour() {
    return HookerNS::OriginalBytesBehaviour::BeforeFunctionCall;
}

void Data::InitFeatures() {
    Features::TEST_FEATURE = (FeaturesNS::TestFeatureNS::Feature*)this->featuresService->GetFeature(FeaturesNS::TestFeatureNS::NAME);
}

BOOL Data::DirectReadWithOffset() {
    return TRUE;
}

RegistersUtils::Register Data::GetRegisterForSafeJump() {
    return RegistersUtils::Register::R13;
}