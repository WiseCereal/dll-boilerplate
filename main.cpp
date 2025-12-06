#include "src/pch/pch.h"

#include "src/services/Console/Service.h"


DWORD __stdcall HackThread(HMODULE dllHandle) {
    ConsoleNS::Service consoleService;

    try {
        consoleService.OpenConsole();

        while (TRUE) { // todo: eject combination keys
            try {
                // loop
            }
            catch (std::exception& e) {
                std::cout << " Exception -> " << e.what() << std::endl;
            }

            Sleep(34);
        };
    }
    catch (const std::exception& e) {
        MessageBoxA(0, e.what(), "", MB_ICONERROR | MB_SYSTEMMODAL);
    }

    consoleService.CloseConsole();
    FreeLibraryAndExitThread(dllHandle, 0);

    return 0;
}

extern "C" __declspec(dllexport) int WINAPI StartHackThread() {
    HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HackThread, GetModuleHandle(NULL), 0, NULL);
    if (!threadHandle) {
        return GetLastError();
    }

    CloseHandle(threadHandle);
    return 0;
}

extern "C" __declspec(dllexport) float GetPlayerHealth() {
    /*
        <Address>"GameAssembly.dll"+0305D3B8</Address>
        <Offset>90</Offset>
        <Offset>60</Offset>
        <Offset>0</Offset>
        <Offset>B8</Offset>
        <Offset>40</Offset>
    */

    return 123;
}

extern "C" __declspec(dllexport) BOOL SetPlayerHealth(float value) {
    return TRUE;
}

BOOL __stdcall DllMain(HMODULE dllHandle, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(dllHandle);
        MessageBoxA(NULL, "DLL Injected", "", MB_OK);
    } break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return true;
}
