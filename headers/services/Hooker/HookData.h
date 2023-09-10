#pragma once

#include "headers/pch.h"
#include "headers/utils/CodingUtils.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"

namespace HookerNS {

    struct x86Registers {
        // Do not change the order. This is the order they get pushed onto the stack with pushad.
        DWORD32 EDI;
        DWORD32 ESI;
        DWORD32 EBP;
        DWORD32 ESP;
        DWORD32 EBX;
        DWORD32 EDX;
        DWORD32 ECX;
        DWORD32 EAX;
    };

    struct x64Registers {
        // Do not change the order. This is the order they get pushed onto the stack with pushad.
    };

    class HookData {
    protected:
        FeaturesHandlerNS::Service* featuresHandler = NULL;
        std::vector<BYTE> bytesToReplace;
        ADDRESS_TYPE bytesToReplaceAddressOffset = 0;
        std::string bytesToReplaceAddressOffsetStr = "";
        BOOL isFindOriginalAddressThreadRunning = FALSE;
        BOOL requestToEndThread = FALSE;

        LPVOID originalAddress = (LPVOID)(ADDRESS_TYPE)0x0000000000000000;
        LPVOID scanStartingAddress = (LPVOID)(ADDRESS_TYPE)0x0000000000000000;
        LPVOID scanEndingAddress = (LPVOID)(ADDRESS_TYPE)0x00007FFFFFFFFFFF;


    public:
        virtual ~HookData() {}
        virtual std::string GetName() = 0;
        virtual ADDRESS_TYPE GetTrampolineRef() = 0;
        virtual void InitFeatures() = 0;

        void Reset();
        std::vector<BYTE> GetBytesToReplace();
        HookData* SetBytesToReplace(std::vector<BYTE> bytes);
        LPVOID GetScanStartingAddress();
        HookData* SetScanStartingAddress(LPVOID address);
        LPVOID GetScanEndingAddress();
        HookData* SetScanEndingAddress(LPVOID address);
        ADDRESS_TYPE GetBytesToReplaceAddressOffset();
        HookData* SetBytesToReplaceAddressOffset(std::string offset);
        BOOL GetIsFindOriginalAddressThreadRunning();
        HookData* SetIsFindOriginalAddressThreadRunning(BOOL v);
        HookData* SetRequestToEndThread();
        BOOL ShouldEndThread();
        LPVOID GetOriginalAddress();
        HookData* SetOriginalAddress(LPVOID addr);
        UINT GetAmountOfBytesToSkipBetweenScans();
        HookData* SetFeaturesHandler(FeaturesHandlerNS::Service* featuresHandler);
    };

}
