#pragma once

#include "headers/pch.h"

namespace AddressUtils {

    ADDRESS_TYPE* GetPointerPathFinalAddress(ADDRESS_TYPE baseAddress, const std::vector<UINT> offsets);

    ADDRESS_TYPE* GetByScanningBytes(ADDRESS_TYPE startAt, ADDRESS_TYPE endAt, UINT skip, std::vector<BYTE> bytesToFind, std::function<BOOL()> shouldStopCallback);

}
