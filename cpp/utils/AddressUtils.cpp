#include "headers/pch.h"

#include "headers/utils/AddressUtils.h"
#include "headers/utils/CodingUtils.h"
#include "headers/exceptions/AccessViolationException.h"


ADDRESS_TYPE* AddressUtils::GetPointerPathFinalAddress(ADDRESS_TYPE baseAddress, const std::vector<UINT> offsets) {
    ADDRESS_TYPE computedAddress = 0x0000000000000000;
    ADDRESS_TYPE nextAddress = (ADDRESS_TYPE)baseAddress;
    for (int offset : offsets) {
        if (!nextAddress) {
            computedAddress = 0x0000000000000000;
            break;
        }

        // Compute the address where we need to read the next address from.
        computedAddress = nextAddress + offset;

        // Now computedAddress points to a region in memory where the nextAddress of the pointer chain is stored.
        try {
            // Read the contents in the memory address stored in computedAddress
            nextAddress = (*(ADDRESS_TYPE*)computedAddress);
        }
        catch (...) {
            CustomExceptionsNS::AccessViolationException ex;
            ex.setWhat("Couldn't read at address: " + CodingUtils::ScalarToHexString<ADDRESS_TYPE>(computedAddress));
            throw ex;
        }
    }

    return (ADDRESS_TYPE*)computedAddress;
}

ADDRESS_TYPE* AddressUtils::GetByScanningBytes(ADDRESS_TYPE startAt, ADDRESS_TYPE endAt, UINT skip, std::vector<BYTE> bytesToFind, std::function<BOOL()> shouldStopCallback) {
    ADDRESS_TYPE currentAddress = startAt;

    HANDLE ownProcessHandle = GetCurrentProcess();
    while (currentAddress < endAt) {
        if (shouldStopCallback()) {
            return 0;
        }

        if (currentAddress + bytesToFind.size() < currentAddress) {
            // Overflow - We didn't find the bytes we were looking for.
            return 0;
        }

        std::vector<BYTE> bytesToRead(bytesToFind.size());
        if (ReadProcessMemory(ownProcessHandle, (LPVOID)currentAddress, bytesToRead.data(), bytesToFind.size(), NULL)) {
            if (bytesToRead == bytesToFind) {
                return (ADDRESS_TYPE*)currentAddress;
            }
        }

        currentAddress = currentAddress + skip;
    }

    return 0;
}