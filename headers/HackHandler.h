#pragma once

#include "headers/pch.h"


namespace HackHandlerNS {

    const DWORD TICKER_IN_MS = 34;
    const std::vector<UINT> EJECT_DLL_KEYS_COMBINATION = { VK_END, VK_ESCAPE, VK_DOWN };

    class Service {
    private:
        BOOL shouldEjectDLL = FALSE;

    public:
        Service();

        void Init();
        void Reset();
        void MainLoop();
        void MarkDLLToBeEjected();
        BOOL ShouldDLLBeEjected();
        BOOL IsReady();

    private:
        BOOL isReady = FALSE;

        void checkEjectDLLKeysCombination();
    };

}
