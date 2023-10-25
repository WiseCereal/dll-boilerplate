#pragma once

#include "pch.h"

namespace AddressUtils {

    ADDRESS_TYPE* GetPointerPathFinalAddress(
        HANDLE processHandle,
        ADDRESS_TYPE baseAddress,
        const std::vector<UINT> offsets
    );

    ADDRESS_TYPE* GetByScanningBytes(
        HANDLE processHandle,
        ADDRESS_TYPE startAt,
        ADDRESS_TYPE endAt,
        UINT skip,
        std::vector<BYTE> bytesToFind,
        std::function<BOOL()> shouldStopCallback
    );

}
