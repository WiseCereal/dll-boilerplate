#include "headers/pch.h"

#include "headers/utils/AddressUtils.h"

#include "headers/services/Hooker/Hooker.h"
#include "headers/services/Hooker/HookData.h"
#include "headers/utils/CodingUtils.h"

using namespace HookerNS;


void Threads::InitAddressesThread(HookData* hook) {
    hook->SetIsFindOriginalAddressThreadRunning(TRUE);

    // if must scan, call getByScanningByes
    // if can use offset, read memory at base addr + offset

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
    }

    s->EnableAllHooks();
    std::cout << " All Hooks Enabled " << std::endl;
}