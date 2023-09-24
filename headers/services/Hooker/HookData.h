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
        std::vector<BYTE> trampolineBytes;
        ADDRESS_TYPE bytesToReplaceAddressOffset = 0;
        std::string bytesToReplaceAddressOffsetStr = "";
        BOOL isFindOriginalAddressThreadRunning = FALSE;
        BOOL requestToEndThread = FALSE;

        std::wstring moduleName = L"Notepad.exe";
        LPVOID originalAddress = (LPVOID)(ADDRESS_TYPE)0x0000000000000000;
        LPVOID scanStartingAddress = (LPVOID)(ADDRESS_TYPE)0x0000000000000000;
        LPVOID scanEndingAddress = (LPVOID)(ADDRESS_TYPE)0x00007FFFFFFFFFFF;
        
        std::vector<BYTE> x32trampolineSkeleton = {
            0x9c, 0x60, // pushf and pusha
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov $register, $custom_dll_function_address
            0x00, 0x00, 0x00, // call $register
            0x61, 0x9d, // popad and popfd
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov $register, $original_address + jmp skeleton size
            0x00, 0x00, 0x00 // jmp $register
        };

        /**
            PUSH ALL REGISTERS // in x64 looks like I have to manually push each one with push rax, push rsi, push r9, etc etc etc
            PUSH FLAGS

            mov rax,0x7fffffffffff // Move the ABSOLUTE address of our DLL c++ function
            CALL rax // Call our dll c++ function. THIS MUST BE A NAKED FUNCTION.

            POP FLAGS
            POP ALL REGISTERS

            MOV rax,0x7fffffffffff // Return address right after we set up the jump to this trampoline (the POP instruction)
            JMP rax
        */
        std::vector<BYTE> x64trampolineSkeleton = {
            0x9c, // pushf,
            0x90, 0x50, // NOP + push rax
            0x90, 0x51, // NOP + push rcx
            0x90, 0x52, // NOP + push rdx
            0x90, 0x53, // NOP + push rbx
            0x90, 0x54, // NOP + push rsp
            0x90, 0x55, // NOP + push rbp
            0x90, 0x56, // NOP + push esi
            0x90, 0x57, // NOP + push rdi
            0x41, 0x50, // push r8,
            0x41, 0x51, // push r9,
            0x41, 0x52, // push r10,
            0x41, 0x53, // push r11,
            0x41, 0x54, // push r12,
            0x41, 0x55, // push r13,
            0x41, 0x56, // push r14,
            0x41, 0x57, // push r15,
        };


    public:
        virtual ~HookData() {}
        virtual std::string GetName() = 0;
        virtual std::vector<BYTE>* GetTrampolineBytes(UINT jmpSkeletonSize) = 0;
        virtual void InitFeatures() = 0;
        virtual BOOL DirectReadWithOffset() = 0;
        virtual RegistersUtils::Register GetRegisterForSafeJump() = 0;

        void Reset();
        std::vector<BYTE> GetBytesToReplace();
        HookData* SetBytesToReplace(std::vector<BYTE> bytes);
        std::vector<BYTE> GetTrampolineBytes();
        HookData* SetTrampolineBytes(std::vector<BYTE> bytes);
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
        HookData* SetFeaturesHandler(FeaturesHandlerNS::Service* featuresHandler);
    };

}
