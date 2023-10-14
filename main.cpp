#include "headers/pch.h"

#include "headers/HackHandler.h"
#include "headers/services/Console/Service.h"
#include "headers/services/Features/Service.h"
#include "headers/services/Addresses/Service.h"
#include "headers/services/Hooker/Service.h"

/******** Services initializations ********/
ConsoleNS::Service ConsoleService;
FeaturesNS::Service FeaturesService;
AddressesNS::Service AddressesService;
HookerNS::Service HookerService(
    CodingUtils::GetTargetArchitecture(),
    &FeaturesService
);

HackHandlerNS::Service HackHandler(
    &HookerService,
    &FeaturesService,
    &AddressesService
);
/******************************************/

__declspec(dllexport) BOOL __stdcall ExecuteFunction(LPVOID argumentsAddress) {
    try {
        std::string functionName = (const char*)argumentsAddress;
        ADDRESS_TYPE argumentAddress = (ADDRESS_TYPE)argumentsAddress + (functionName.length() + 1);

        if (functionName == "IsDLLReady") {
            return HackHandler.IsReady();
        }

        if (!HackHandler.IsReady()) {
            std::cout << " DLL is not ready - Disallowed to call DLL functions until it's ready " << std::endl;
            return FALSE;
        }

        if (functionName == "EjectDLL") {
            HackHandler.Reset();
            return HackHandler.MarkDLLToBeEjected();
        }

        if (functionName == "EnableFeature") {
            FeaturesService.EnableFeature((const char*)argumentAddress);
            return TRUE;
        }

        if (functionName == "DisableFeature") {
            FeaturesService.DisableFeature((const char*)argumentAddress);
            return TRUE;
        }

        if (functionName == "SetVariableValue") {
            std::string variableName = (const char*)argumentAddress;
            ADDRESS_TYPE valueAddress = argumentAddress + variableName.length() + 1;
            AddressesService.SetVariableValue(variableName, valueAddress);
        }

        throw std::exception("Invalid function name");
    }
    catch (std::exception& e) {
        std::cout << " Error -> " << e.what() << std::endl;
        return FALSE;
    }
}

DWORD __stdcall HackThread(HMODULE dllHandle) {
    try {
        ConsoleService.OpenConsole();

        HackHandler.Init();
        while (!HackHandler.ShouldDLLBeEjected()) {
            try {
                HackHandler.MainLoop();
            }
            catch (std::exception& e) {
                std::cout << " Exception -> " << e.what() << std::endl;
            }

            Sleep(HackHandlerNS::TICKER_IN_MS);
        };
    }
    catch (const std::exception& e) {
        MessageBoxA(0, e.what(), "", MB_ICONERROR | MB_SYSTEMMODAL);
    }

    ConsoleService.CloseConsole();
    FreeLibraryAndExitThread(dllHandle, 0);

    return 0;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        auto thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, NULL);
        if (thread) {
            CloseHandle(thread);
        }
    } break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return true;
}
