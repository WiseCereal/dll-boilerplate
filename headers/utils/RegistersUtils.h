#pragma once

#include "headers/pch.h"

namespace RegistersUtils {

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
        ULONG64 RDI;
        ULONG64 RSI;
        ULONG64 RBP;
        ULONG64 RSP;
        ULONG64 RBX;
        ULONG64 RDX;
        ULONG64 RCX;
        ULONG64 RAX;
        ULONG64 R8;
        ULONG64 R9;
        ULONG64 R10;
        ULONG64 R11;
        ULONG64 R12;
        ULONG64 R13;
        ULONG64 R14;
        ULONG64 R15;
    };

    enum class Register {
        EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI,
        RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15
    };

    std::vector<BYTE> MOVInstructionBytes(Register r);

    std::vector<BYTE> JMPInstructionBytes(Register r);

    std::vector<BYTE> PUSHInstructionBytes(Register r);

    std::vector<BYTE> POPInstructionBytes(Register r);

    std::vector<BYTE> CALLInstructionBytes(Register r);

}
