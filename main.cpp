#include "headers/pch.h"

#include "headers/HackHandler.h"
#include "headers/services/ConsoleHandler.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/Hooker.h"

#define EXPOSE __declspec(dllexport) BOOL __stdcall

/******** Services initializations ********/
ConsoleHandlerNS::Handler ConsoleHandler;
FeaturesHandlerNS::Service FeaturesHandler;
HookerNS::Service HookerService(
    CodingUtils::GetTargetArchitecture(),
    &FeaturesHandler
);

HackHandlerNS::Service HackHandler(
    &HookerService,
    &FeaturesHandler
);
/******************************************/


BOOL TestIntArgument(UINT arg) {
    std::cout << "Int argument is " << arg << std::endl;
    return TRUE;
}

BOOL TestStringArgument(std::string arg) {
    std::cout << "String argument is " << arg << std::endl;
    return TRUE;
}

BOOL TestMultipleArguments(ADDRESS_TYPE argumentsAddress) {
    std::string argument1 = (const char*)argumentsAddress;

    ADDRESS_TYPE argument2address = (ADDRESS_TYPE)argumentsAddress + (argument1.length() + 1);
    UINT argument2 = *(UINT*)argument2address;

    ADDRESS_TYPE argument3address = (ADDRESS_TYPE)argument2address + (sizeof(argument2));
    double argument3 = *(double*)argument3address;

    ADDRESS_TYPE argument4address = (ADDRESS_TYPE)argument3address + (sizeof(argument3));
    float argument4 = *(float*)argument4address;

    std::cout << " Argument 1 is " << argument1 << " | " << std::hex << &argument1 << std::endl;
    std::cout << " Argument 2 is " << std::dec << argument2 << " | " << std::hex << &argument2 << std::endl;
    std::cout << " Argument 3 is " << argument3 << " | " << std::hex << &argument3 << std::endl;
    std::cout << " Argument 4 is " << argument4 << " | " << std::hex << &argument4 << std::endl;

    return TRUE;
}

BOOL EjectDLL() {
    HackHandler.Reset();
    return HackHandler.MarkDLLToBeEjected();
}

BOOL IsDLLReady() {
    return HackHandler.IsReady();
}

EXPOSE ExecuteFunction(LPVOID argumentsAddress) {
    try {
        std::string functionName = (const char*)argumentsAddress;
        ADDRESS_TYPE argumentAddress = (ADDRESS_TYPE)argumentsAddress + (functionName.length() + 1);

        if (functionName == "IsDLLReady") {
            return IsDLLReady();
        }

        if (!HackHandler.IsReady()) {
            std::cout << " DLL is not ready - Disallowed to call DLL functions until it's ready " << std::endl;
            return FALSE;
        }

        if (functionName == "EjectDLL") {
            return EjectDLL();
        }

        if (functionName == "TestIntArgument") {
            return TestIntArgument(*(UINT*)argumentAddress);
        }

        if (functionName == "TestStringArgument") {
            return TestStringArgument((const char*)argumentAddress);
        }

        if (functionName == "TestMultipleArguments") {
            return TestMultipleArguments(argumentAddress);
        }

        throw std::exception("Invalid function name");
    }
    catch (std::exception& e) {
        std::cout << " Error -> " << e.what() << std::endl;
        return FALSE;
    }
}

DWORD __stdcall HackThread(HMODULE h) {
    try {
        ConsoleHandler.OpenConsole();

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

    ConsoleHandler.CloseConsole();
    FreeLibraryAndExitThread(h, 0);

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
