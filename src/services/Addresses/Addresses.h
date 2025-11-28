#pragma once

#include "src/pch/pch.h"

namespace AddressesNS {

    namespace Keys {
        static const std::string HEALTH = "health";
        static const std::string MAX_HEALTH = "maxHealth";
    }

    static const std::map<std::string, std::vector<UINT>> OFFSETS = {
        {Keys::HEALTH, {0x48184, 0x480, 0x14, 0x78, 0x68, 0x3D8, 0xC, 0x12C}},
        {Keys::MAX_HEALTH, {0x48184, 0x480, 0x14, 0x78, 0x68, 0x3D8, 0xC, 0x134}},
    };

    enum class VariableType { UINT32, INT32, DOUBLE, FLOAT, BOOL };

    static const std::map<std::string, VariableType> VARIABLE_TYPES = {
        {Keys::HEALTH, VariableType::UINT32},
        {Keys::MAX_HEALTH, VariableType::UINT32},
    };

}