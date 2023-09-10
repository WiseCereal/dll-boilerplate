#pragma once

#include "headers/pch.h"

#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/HookData.h"
#include "headers/hooks/TestHook.h"

namespace HookerNS {

    class Service {
    private:
        FeaturesHandlerNS::Service* featuresHandler;

        HooksNS::TestHook::Data testHook;

        std::vector<HookData*> hooksVector;
        std::vector<HANDLE> initHookThreads;

    public:
        Service(
            FeaturesHandlerNS::Service* featuresHandler
        );

        Service* SetBaseAddress(LPVOID addr);
        LPVOID GetBaseAddress();
        Service* InitHooks();
        BOOL AreThereAnyInitAddressThreadsRunning();
        Service* Reset();
        Service* EnableHook(std::string hookName);
        Service* DisableHook(std::string hookName);
        Service* EnableAllHooks();
        Service* DisableAllHooks();
        std::vector<HookData*> GetHooksVector();
        std::vector<HANDLE> GetInitHookThreads();
        FeaturesHandlerNS::Service* GetFeaturesHandler();

    private:
        Service* validateHooks();
        HookData* findHook(std::string hookName);
        Service* _enableHook(HookData* hookData);
        Service* _disableHook(HookData* hookData);
    };

    namespace Threads {

        void InitAddressesThread(HookData* hook);

        void OnInitAddressesThreadFinish(Service* s);

    }

}
