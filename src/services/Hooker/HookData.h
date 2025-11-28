#pragma once

#include "src/pch/pch.h"
#include "src/utils/CodingUtils.h"
#include "src/utils/RegistersUtils.h"
#include "src/services/Features/Service.h"

namespace HookerNS {

    enum class OriginalBytesBehaviour {
        BeforeFunctionCall,
        AfterFunctionCall,
        Remove
    };

    class HookData {
    protected:
        FeaturesNS::Service* featuresService = NULL;
        std::vector<BYTE> bytesToReplace;
        std::vector<BYTE> trampolineBytes;
        ADDRESS_TYPE bytesToReplaceAddressOffset = 0;
        std::string bytesToReplaceAddressOffsetStr = "";
        BOOL isFindOriginalAddressThreadRunning = FALSE;
        BOOL requestToEndThread = FALSE;

        std::wstring moduleName = L"Notepad.exe";
        LPVOID originalAddress = (LPVOID)(ADDRESS_TYPE)0x0000000000000000;
        LPVOID scanStartingAddress = (LPVOID)(ADDRESS_TYPE)0x0000000000000000;
        LPVOID scanEndingAddress = (LPVOID)(ADDRESS_TYPE)0x00007FFFFFFFFFFF;
        
    public:
        virtual ~HookData() {}
        virtual std::string GetName() = 0;
        virtual void InitFeatures() = 0;
        virtual BOOL DirectReadWithOffset() = 0;
        virtual RegistersUtils::Register GetRegisterForSafeJump() = 0;
        virtual ADDRESS_TYPE GetHookFunctionAddress() = 0;
        virtual OriginalBytesBehaviour GetOriginalBytesBehaviour() = 0;

        void Reset();
        HookData* SetTrampolineBytes(std::vector<BYTE> trampolineBytes);
        std::vector<BYTE>* GetTrampolineBytes();
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
        HookData* SetModuleName(std::wstring moduleName);
        std::wstring GetModuleName();
        LPVOID GetOriginalAddress();
        HookData* SetOriginalAddress(LPVOID addr);
        UINT GetAmountOfBytesToSkipBetweenScans();
        HookData* SetFeaturesService(FeaturesNS::Service* featuresService);
    };

}
