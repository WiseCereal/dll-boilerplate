#include "src/pch/pch.h"

#include "src/utils/RegistersUtils.h"


std::vector<BYTE> RegistersUtils::MOVInstructionBytes(Register r) {
    std::vector<BYTE> bytesVector;

    switch (r) {
    case Register::EAX: bytesVector = { 0x90, 0xB8 }; break;
    case Register::ECX: bytesVector = { 0x90, 0xB9 }; break;
    case Register::EDX: bytesVector = { 0x90, 0xBA }; break;
    case Register::EBX: bytesVector = { 0x90, 0xBB }; break;
    case Register::ESP: bytesVector = { 0x90, 0xBC }; break;
    case Register::EBP: bytesVector = { 0x90, 0xBD }; break;
    case Register::ESI: bytesVector = { 0x90, 0xBE }; break;
    case Register::EDI: bytesVector = { 0x90, 0xBF }; break;

    case Register::RAX: bytesVector = { 0x48, 0xB8 }; break;
    case Register::RCX: bytesVector = { 0x48, 0xB9 }; break;
    case Register::RDX: bytesVector = { 0x48, 0xBA }; break;
    case Register::RBX: bytesVector = { 0x48, 0xBB }; break;
    case Register::RSP: bytesVector = { 0x48, 0xBC }; break;
    case Register::RBP: bytesVector = { 0x48, 0xBD }; break;
    case Register::RSI: bytesVector = { 0x48, 0xBE }; break;
    case Register::RDI: bytesVector = { 0x48, 0xBF }; break;

    case Register::R8: bytesVector = { 0x49, 0xB8 }; break;
    case Register::R9: bytesVector = { 0x49, 0xB9 }; break;
    case Register::R10: bytesVector = { 0x49, 0xBA }; break;
    case Register::R11: bytesVector = { 0x49, 0xBB }; break;
    case Register::R12: bytesVector = { 0x49, 0xBC }; break;
    case Register::R13: bytesVector = { 0x49, 0xBD }; break;
    case Register::R14: bytesVector = { 0x49, 0xBE }; break;
    case Register::R15: bytesVector = { 0x49, 0xBF }; break;

    default:
        throw std::runtime_error("Invalid register");
    }

    return bytesVector;
}


std::vector<BYTE> RegistersUtils::JMPInstructionBytes(Register r) {
    std::vector<BYTE> bytesVector;

    switch (r) {
    case Register::EAX: case Register::RAX: bytesVector = { 0x90, 0xFF, 0xE0 }; break;
    case Register::ECX: case Register::RCX: bytesVector = { 0x90, 0xFF, 0xE1 }; break;
    case Register::EDX: case Register::RDX: bytesVector = { 0x90, 0xFF, 0xE2 }; break;
    case Register::EBX: case Register::RBX: bytesVector = { 0x90, 0xFF, 0xE3 }; break;
    case Register::ESP: case Register::RSP: bytesVector = { 0x90, 0xFF, 0xE4 }; break;
    case Register::EBP: case Register::RBP: bytesVector = { 0x90, 0xFF, 0xE5 }; break;
    case Register::ESI: case Register::RSI: bytesVector = { 0x90, 0xFF, 0xE6 }; break;
    case Register::EDI: case Register::RDI: bytesVector = { 0x90, 0xFF, 0xE7 }; break;

    case Register::R8: bytesVector = { 0x41, 0xFF, 0xE0 }; break;
    case Register::R9: bytesVector = { 0x41, 0xFF, 0xE1 }; break;
    case Register::R10: bytesVector = { 0x41, 0xFF, 0xE2 }; break;
    case Register::R11: bytesVector = { 0x41, 0xFF, 0xE3 }; break;
    case Register::R12: bytesVector = { 0x41, 0xFF, 0xE4 }; break;
    case Register::R13: bytesVector = { 0x41, 0xFF, 0xE5 }; break;
    case Register::R14: bytesVector = { 0x41, 0xFF, 0xE6 }; break;
    case Register::R15: bytesVector = { 0x41, 0xFF, 0xE7 }; break;

    default:
        throw std::runtime_error("Invalid register");
    }

    return bytesVector;
}


std::vector<BYTE> RegistersUtils::PUSHInstructionBytes(Register r) {
    std::vector<BYTE> bytesVector;

    switch (r) {
    case Register::EAX: case Register::RAX: bytesVector = { 0x90, 0x50 }; break;
    case Register::ECX: case Register::RCX: bytesVector = { 0x90, 0x51 }; break;
    case Register::EDX: case Register::RDX: bytesVector = { 0x90, 0x52 }; break;
    case Register::EBX: case Register::RBX: bytesVector = { 0x90, 0x53 }; break;
    case Register::ESP: case Register::RSP: bytesVector = { 0x90, 0x54 }; break;
    case Register::EBP: case Register::RBP: bytesVector = { 0x90, 0x55 }; break;
    case Register::ESI: case Register::RSI: bytesVector = { 0x90, 0x56 }; break;
    case Register::EDI: case Register::RDI: bytesVector = { 0x90, 0x57 }; break;

    case Register::R8: bytesVector = { 0x41, 0x50 }; break;
    case Register::R9: bytesVector = { 0x41, 0x51 }; break;
    case Register::R10: bytesVector = { 0x41, 0x52 }; break;
    case Register::R11: bytesVector = { 0x41, 0x53 }; break;
    case Register::R12: bytesVector = { 0x41, 0x54 }; break;
    case Register::R13: bytesVector = { 0x41, 0x55 }; break;
    case Register::R14: bytesVector = { 0x41, 0x56 }; break;
    case Register::R15: bytesVector = { 0x41, 0x57 }; break;

    default:
        throw std::runtime_error("Invalid register");
    }

    return bytesVector;
}


std::vector<BYTE> RegistersUtils::POPInstructionBytes(Register r) {
    std::vector<BYTE> bytesVector;

    switch (r) {
    case Register::EAX: case Register::RAX: bytesVector = { 0x90, 0x58 }; break;
    case Register::ECX: case Register::RCX: bytesVector = { 0x90, 0x59 }; break;
    case Register::EDX: case Register::RDX: bytesVector = { 0x90, 0x5A }; break;
    case Register::EBX: case Register::RBX: bytesVector = { 0x90, 0x5B }; break;
    case Register::ESP: case Register::RSP: bytesVector = { 0x90, 0x5C }; break;
    case Register::EBP: case Register::RBP: bytesVector = { 0x90, 0x5D }; break;
    case Register::ESI: case Register::RSI: bytesVector = { 0x90, 0x5E }; break;
    case Register::EDI: case Register::RDI: bytesVector = { 0x90, 0x5F }; break;

    case Register::R8: bytesVector = { 0x41, 0x58 }; break;
    case Register::R9: bytesVector = { 0x41, 0x59 }; break;
    case Register::R10: bytesVector = { 0x41, 0x5A }; break;
    case Register::R11: bytesVector = { 0x41, 0x5B }; break;
    case Register::R12: bytesVector = { 0x41, 0x5C }; break;
    case Register::R13: bytesVector = { 0x41, 0x5D }; break;
    case Register::R14: bytesVector = { 0x41, 0x5E }; break;
    case Register::R15: bytesVector = { 0x41, 0x5F }; break;

    default:
        throw std::runtime_error("Invalid register");
    }

    return bytesVector;
}

std::vector<BYTE> RegistersUtils::CALLInstructionBytes(Register r) {
    std::vector<BYTE> bytesVector;

    switch (r) {
    case Register::EAX: case Register::RAX: bytesVector = { 0x90, 0xFF, 0xD0 }; break;
    case Register::ECX: case Register::RCX: bytesVector = { 0x90, 0xFF, 0xD1 }; break;
    case Register::EDX: case Register::RDX: bytesVector = { 0x90, 0xFF, 0xD2 }; break;
    case Register::EBX: case Register::RBX: bytesVector = { 0x90, 0xFF, 0xD3 }; break;
    case Register::ESP: case Register::RSP: bytesVector = { 0x90, 0xFF, 0xD4 }; break;
    case Register::EBP: case Register::RBP: bytesVector = { 0x90, 0xFF, 0xD5 }; break;
    case Register::ESI: case Register::RSI: bytesVector = { 0x90, 0xFF, 0xD6 }; break;
    case Register::EDI: case Register::RDI: bytesVector = { 0x90, 0xFF, 0xD7 }; break;

    case Register::R8: bytesVector = { 0x41, 0xFF, 0xD0 }; break;
    case Register::R9: bytesVector = { 0x41, 0xFF, 0xD1 }; break;
    case Register::R10: bytesVector = { 0x41, 0xFF, 0xD2 }; break;
    case Register::R11: bytesVector = { 0x41, 0xFF, 0xD3 }; break;
    case Register::R12: bytesVector = { 0x41, 0xFF, 0xD4 }; break;
    case Register::R13: bytesVector = { 0x41, 0xFF, 0xD5 }; break;
    case Register::R14: bytesVector = { 0x41, 0xFF, 0xD6 }; break;
    case Register::R15: bytesVector = { 0x41, 0xFF, 0xD7 }; break;

    default:
        throw std::runtime_error("Invalid register");
    }

    return bytesVector;
}

