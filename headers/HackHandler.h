#pragma once

#include "headers/pch.h"

#include "headers/services/Features/Service.h"
#include "headers/services/Hooker/Service.h"

namespace HackHandlerNS {

    const DWORD TICKER_IN_MS = 34;
    const std::vector<UINT> EJECT_DLL_KEYS_COMBINATION = { VK_END, VK_ESCAPE, VK_DOWN };

    class Service {
    private:
        HookerNS::Service* hookerService;
        FeaturesNS::Service* featuresService;
        BOOL shouldEjectDLL = FALSE;

    public:
        Service(
            HookerNS::Service* hookerService,
            FeaturesNS::Service* featuresService
        );

        void Init();
        void Reset();
        void MainLoop();
        BOOL MarkDLLToBeEjected();
        BOOL ShouldDLLBeEjected();
        BOOL IsReady();
        void MarkAsReady();
        HookerNS::Service* GetHookerService();

    private:
        BOOL isReady = FALSE;

        void checkEjectDLLKeysCombination();
    };

}
