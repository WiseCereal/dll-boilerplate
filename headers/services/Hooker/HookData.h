#pragma once

#include "headers/pch.h"
#include "headers/utils/CodingUtils.h"
#include "headers/utils/RegistersUtils.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"

namespace HookerNS {

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
        virtual BOOL DirectReadWithOffset() = 0;
        virtual RegistersUtils::Register GetRegisterForSafeJump() = 0;

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
