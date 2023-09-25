#include "headers/pch.h"

#include "headers/utils/AddressUtils.h"

#include "headers/services/Hooker/Hooker.h"
#include "headers/services/Hooker/HookData.h"
#include "headers/utils/CodingUtils.h"
#include "headers/services/Hooker/TrampolineSkeletons.h"

using namespace HookerNS;


void Threads::InitAddressesThread(HookData* hook) {
    hook->SetIsFindOriginalAddressThreadRunning(TRUE);

    LPVOID baseModuleAddress = GetModuleHandle(hook->GetModuleName().data());

    if (hook->DirectReadWithOffset()) {
        hook->SetOriginalAddress(
            (LPVOID)((ADDRESS_TYPE)baseModuleAddress + hook->GetBytesToReplaceAddressOffset())
        );
    }
    else {
        hook->SetScanStartingAddress(baseModuleAddress);
        auto addr = AddressUtils::GetByScanningBytes(
            (ADDRESS_TYPE)hook->GetScanStartingAddress() + hook->GetBytesToReplaceAddressOffset(),
            (ADDRESS_TYPE)hook->GetScanEndingAddress(),
            hook->GetAmountOfBytesToSkipBetweenScans(),
            hook->GetBytesToReplace(),
            [hook]() {
                return hook->ShouldEndThread();
            }
        );
        hook->SetOriginalAddress(addr);
    }

    hook->SetIsFindOriginalAddressThreadRunning(FALSE);
}

void Threads::OnInitAddressesThreadFinish(Service* s) {
    auto threadsVector = s->GetInitHookThreads();

    DWORD size = (DWORD)threadsVector.size();
    WaitForMultipleObjects(size, threadsVector.data(), TRUE, INFINITE);
    for (HANDLE h : threadsVector) {
        CloseHandle(h);
    }

    for (auto hook : s->GetHooksVector()) {
        if (hook->ShouldEndThread()) {
            continue;
        }
        hook->SetFeaturesHandler(s->GetFeaturesHandler());
        hook->InitFeatures();
        hook->PrepareTrampolineBytes(
            GetTrampolineSkeleton(hook->GetArchitecture()),
            s->GetJMPSkeleton().size()
        );
    }

    s->EnableAllHooks();
    std::cout << " All Hooks Enabled " << std::endl;
}

std::vector<BYTE> Threads::GetTrampolineSkeleton(UINT architecture) {
    switch (architecture) {
    case 0x86:
        return TrampolineSkeletons::x86;
    case 0x64:
        return TrampolineSkeletons::x64;
    default:
        throw std::exception("Invalid architecture");
    }
}