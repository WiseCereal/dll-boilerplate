#pragma once

#include "pch.h"

#include "src/services/Features/Service.h"
#include "src/services/Addresses/Service.h"
#include "src/services/Hooker/Service.h"

namespace HackHandlerNS {

    const DWORD TICKER_IN_MS = 34;
    const std::vector<UINT> EJECT_DLL_KEYS_COMBINATION = { VK_END, VK_ESCAPE, VK_DOWN };

    class Service {
    private:
        HookerNS::Service* hookerService;
        FeaturesNS::Service* featuresService;
        AddressesNS::Service* addressesService;
        BOOL shouldEjectDLL = FALSE;

    public:
        Service(
            HookerNS::Service* hookerService,
            FeaturesNS::Service* featuresService,
            AddressesNS::Service* addressesService
        );

        void Init();
        void Reset();
        void MainLoop();
        BOOL MarkDLLToBeEjected();
        BOOL ShouldDLLBeEjected();
        BOOL IsReady();
        void MarkAsReady();
        HookerNS::Service* GetHookerService();
        FeaturesNS::Service* GetFeaturesService();
        AddressesNS::Service* GetAddressesService();

    private:
        BOOL isReady = FALSE;

        void checkEjectDLLKeysCombination();
    };

}
