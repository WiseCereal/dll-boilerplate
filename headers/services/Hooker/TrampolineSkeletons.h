#pragma once

#include "headers/pch.h"

namespace HookerNS {

    namespace TrampolineSkeletons {

        const std::vector<BYTE> x86 = {
            0x9c, 0x60, // pushf and pusha
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov $register, $custom_dll_function_address
            0x00, 0x00, 0x00, // call $register
            0x61, 0x9d, // popad and popfd
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov $register, $original_address + jmp skeleton size (MUST RETURN TO THE POP INSTRUCTION)
            0x00, 0x00, 0x00 // jmp $register
        };

        const std::vector<BYTE> x64 = {
            0x9C, // push flags,
            0x90, 0x50, // NOP + push rax
            0x90, 0x51, // NOP + push rcx
            0x90, 0x52, // NOP + push rdx
            0x90, 0x53, // NOP + push rbx
            0x90, 0x54, // NOP + push rsp
            0x90, 0x55, // NOP + push rbp
            0x90, 0x56, // NOP + push rsi
            0x90, 0x57, // NOP + push rdi
            0x41, 0x50, // push r8,
            0x41, 0x51, // push r9,
            0x41, 0x52, // push r10,
            0x41, 0x53, // push r11,
            0x41, 0x54, // push r12,
            0x41, 0x55, // push r13,
            0x41, 0x56, // push r14,
            0x41, 0x57, // push r15,

            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov $register, $address
            0x00, 0x00, 0x00, // call $register

            0x41, 0x5F, // pop r15,
            0x41, 0x5E, // pop r14,
            0x41, 0x5D, // pop r13,
            0x41, 0x5C, // pop r12,
            0x41, 0x5B, // pop r11,
            0x41, 0x5A, // pop r10,
            0x41, 0x59, // pop r9,
            0x41, 0x58, // pop r8,
            0x90, 0x5F, // NOP + pop rdi,
            0x90, 0x5E, // NOP + pop rsi,
            0x90, 0x5D, // NOP + pop rbp,
            0x90, 0x5C, // NOP + pop rsp,
            0x90, 0x5B, // NOP + pop rbx,
            0x90, 0x5A, // NOP + pop rdx,
            0x90, 0x59, // NOP + pop rcx,
            0x90, 0x58, // NOP + pop rax
            0x9D, // pop flags

            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov $register, $original_address + jmp skeleton size (MUST RETURN TO THE POP INSTRUCTION)
            0x00, 0x00, 0x00, // jmp $register
        };
    }
}