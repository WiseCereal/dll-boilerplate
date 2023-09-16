#include "headers/pch.h"

#include "headers/utils/RegistersUtils.h"


BYTE RegistersUtils::MOVInstructionByte(Register r) {
    switch (r) {
    case Register::EAX: case Register::RAX: return 0xB8;
    case Register::ECX: case Register::RCX: return 0xB9;
    case Register::EDX: case Register::RDX: return 0xBA;
    case Register::EBX: case Register::RBX: return 0xBB;
    case Register::ESP: case Register::RSP: return 0xBC;
    case Register::EBP: case Register::RBP: return 0xBD;
    case Register::ESI: case Register::RSI: return 0xBE;
    case Register::EDI: case Register::RDI: return 0xBF;

    default:
        throw std::runtime_error("Invalid register");
    }
}


BYTE RegistersUtils::JMPInstructionByte(Register r) {
    switch (r) {
    case Register::EAX: case Register::RAX: return 0xE0;
    case Register::ECX: case Register::RCX: return 0xE1;
    case Register::EDX: case Register::RDX: return 0xE2;
    case Register::EBX: case Register::RBX: return 0xE3;
    case Register::ESP: case Register::RSP: return 0xE4;
    case Register::EBP: case Register::RBP: return 0xE5;
    case Register::ESI: case Register::RSI: return 0xE6;
    case Register::EDI: case Register::RDI: return 0xE7;

    default:
        throw std::runtime_error("Invalid register");
    }
}


BYTE RegistersUtils::PUSHInstructionByte(Register r) {
    switch (r) {
    case Register::EAX: case Register::RAX: return 0x50;
    case Register::ECX: case Register::RCX: return 0x51;
    case Register::EDX: case Register::RDX: return 0x52;
    case Register::EBX: case Register::RBX: return 0x53;
    case Register::ESP: case Register::RSP: return 0x54;
    case Register::EBP: case Register::RBP: return 0x55;
    case Register::ESI: case Register::RSI: return 0x56;
    case Register::EDI: case Register::RDI: return 0x57;

    default:
        throw std::runtime_error("Invalid register");
    }
}


BYTE RegistersUtils::POPInstructionByte(Register r) {
    switch (r) {
    case Register::EAX: case Register::RAX: return 0x58;
    case Register::ECX: case Register::RCX: return 0x59;
    case Register::EDX: case Register::RDX: return 0x5A;
    case Register::EBX: case Register::RBX: return 0x5B;
    case Register::ESP: case Register::RSP: return 0x5C;
    case Register::EBP: case Register::RBP: return 0x5D;
    case Register::ESI: case Register::RSI: return 0x5E;
    case Register::EDI: case Register::RDI: return 0x5F;

    default:
        throw std::runtime_error("Invalid register");
    }
}

