#pragma once

#include "src/pch/pch.h"

#include "src/services/Features/Service.h"
#include "src/services/Hooker/HookData.h"
#include "src/hooks/TestHook.h"

namespace HookerNS {
    class Service {
    private:
        FeaturesNS::Service* featuresService;
        UINT architecture;

        HooksNS::TestHook::Data testHook;

        std::map<std::string, HookData*> hooksMap;
        std::vector<HANDLE> initHookThreads;
        std::vector<BYTE> jmpSkeleton;
        std::vector<BYTE> trampolineSkeleton;

    public:
        Service(
            UINT architecture,
            FeaturesNS::Service* featuresService
        );

        Service* InitHooks(std::function<void()> onFinishCallback);
        BOOL AreThereAnyInitAddressThreadsRunning();
        Service* Reset();
        Service* EnableHook(std::string hookName);
        Service* DisableHook(std::string hookName);
        Service* EnableAllHooks();
        Service* DisableAllHooks();
        std::map<std::string, HookData*>* GetHooksMap();
        std::vector<HANDLE> GetInitHookThreads();
        FeaturesNS::Service* GetFeaturesService();

    private:
        Service* initSkeletons();
        Service* validateHooks();
        HookData* getHook(std::string hookName);
        Service* _enableHook(HookData* hookData);
        Service* _disableHook(HookData* hookData);
        std::vector<BYTE> prepareTrampolineBytes(HookData* hookData);
    };

    namespace Threads {

        void InitAddressesThread(HookData* hook);

        void OnInitAddressesThreadFinish(Service* s);

    }

}
