#include "headers/pch.h"

#include "headers/utils/AddressUtils.h"
#include "headers/utils/CodingUtils.h"

ADDRESS_TYPE* AddressUtils::GetPointerPathFinalAddress(HANDLE processHandle, ADDRESS_TYPE baseAddress, const std::vector<UINT> offsets) {
    ADDRESS_TYPE computedAddress = 0x0;
    ADDRESS_TYPE nextAddress = baseAddress;
    for (UINT offset : offsets) {
        computedAddress = nextAddress + offset;
        if (!ReadProcessMemory(processHandle, (LPVOID)computedAddress, &nextAddress, sizeof(ADDRESS_TYPE), NULL)) {
            return 0x0;
        }
    }

    return (ADDRESS_TYPE*)computedAddress;
}

ADDRESS_TYPE* AddressUtils::GetByScanningBytes(HANDLE processHandle, ADDRESS_TYPE startAt, ADDRESS_TYPE endAt, UINT skip, std::vector<BYTE> bytesToFind, std::function<BOOL()> shouldStopCallback) {
    ADDRESS_TYPE currentAddress = startAt;
    while (currentAddress < endAt) {
        if (shouldStopCallback()) {
            return 0;
        }

        if (currentAddress + bytesToFind.size() < currentAddress) {
            // Overflow - We didn't find the bytes we were looking for.
            return 0;
        }

        std::vector<BYTE> bytesToRead(bytesToFind.size());
        if (ReadProcessMemory(processHandle, (LPVOID)currentAddress, bytesToRead.data(), bytesToFind.size(), NULL)) {
            if (bytesToRead == bytesToFind) {
                return (ADDRESS_TYPE*)currentAddress;
            }
        }

        currentAddress = currentAddress + skip;
    }

    return 0;
}