#include "headers/pch.h"

#include "headers/services/Hooker/HookData.h"
#include "headers/utils/CodingUtils.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"

using namespace HookerNS;


void HookData::Reset() {
    this->originalAddress = (LPVOID)(ADDRESS_TYPE)0x0000000000000000;
    this->scanStartingAddress = (LPVOID)(ADDRESS_TYPE)0x0000000000000000;
    this->scanEndingAddress = (LPVOID)(ADDRESS_TYPE)0xFFFFFFFFFFFFFFFF;
}

std::vector<BYTE> HookData::GetBytesToReplace() {
    return this->bytesToReplace;
}

HookData* HookData::SetBytesToReplace(std::vector<BYTE> bytes) {
    this->bytesToReplace = bytes;
    return this;
}


LPVOID HookData::GetScanStartingAddress() {
    return this->scanStartingAddress;
}
HookData* HookData::SetScanStartingAddress(LPVOID address) {
    this->scanStartingAddress = address;
    return this;
}


LPVOID HookData::GetScanEndingAddress() {
    return this->scanEndingAddress;
}
HookData* HookData::SetScanEndingAddress(LPVOID address) {
    this->scanEndingAddress = address;
    return this;
}


ADDRESS_TYPE HookData::GetBytesToReplaceAddressOffset() {
    return this->bytesToReplaceAddressOffset;
}
HookData* HookData::SetBytesToReplaceAddressOffset(std::string offset) {
    this->bytesToReplaceAddressOffsetStr = offset;
    this->bytesToReplaceAddressOffset = CodingUtils::HexStringToLongLong(offset);
    return this;
}


BOOL HookData::GetIsFindOriginalAddressThreadRunning() {
    return this->isFindOriginalAddressThreadRunning;
}
HookData* HookData::SetIsFindOriginalAddressThreadRunning(BOOL v) {
    this->isFindOriginalAddressThreadRunning = v;
    return this;
}

HookData* HookData::SetRequestToEndThread() {
    this->requestToEndThread = TRUE;
    return this;
}
BOOL HookData::ShouldEndThread() {
    return this->requestToEndThread;
}


LPVOID HookData::GetOriginalAddress() {
    return this->originalAddress;
}
HookData* HookData::SetOriginalAddress(LPVOID addr) {
    this->originalAddress = addr;
    return this;
}

UINT HookData::GetAmountOfBytesToSkipBetweenScans() {
    std::string skipStr = "0x1";
    for (size_t i = 0; i < this->bytesToReplaceAddressOffsetStr.size() - 3; i++) {
        skipStr += "0";
    }

    return CodingUtils::HexStringToInt(skipStr);
}

HookData* HookData::SetFeaturesHandler(FeaturesHandlerNS::Service* featuresHandler) {
    this->featuresHandler = featuresHandler;

    return this;
}
