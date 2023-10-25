#include "pch.h"

#include "src/utils/AddressUtils.h"

#include "src/services/Hooker/Service.h"
#include "src/services/Hooker/HookData.h"
#include "src/utils/CodingUtils.h"

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
            GetCurrentProcess(),
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

    auto hooksMap = s->GetHooksMap();
    for (auto i = hooksMap->begin(); i != hooksMap->end(); i++) {
        std::string hookName = i->first;
        HookData* hook = i->second;

        if (hook->ShouldEndThread()) {
            continue;
        }
        hook->SetFeaturesService(s->GetFeaturesService());
        hook->InitFeatures();
    }

    s->EnableAllHooks();
}
