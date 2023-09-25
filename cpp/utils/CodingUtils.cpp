#include "headers/pch.h"

#include "headers/utils/CodingUtils.h"

bool CodingUtils::AreStringsEqual(std::string str1, std::string str2) {
    return str1.compare(str2) == 0;
}

bool CodingUtils::AreWStringsEqual(std::wstring str1, std::wstring str2) {
    return str1.compare(str2) == 0;
}

bool CodingUtils::StringContainsString(std::string haystack, std::string needle) {
    return haystack.find(needle) != std::string::npos;
}

bool CodingUtils::WStringContainsWString(std::wstring haystack, std::wstring needle) {
    return haystack.find(needle) != std::wstring::npos;
}

void CodingUtils::ByteArrayReplace(size_t index, std::vector<BYTE>* replacement, std::vector<BYTE>* byteArray) {
    UINT j = 0;
    for (size_t i = index; i < index + replacement->size(); i++) {
        byteArray->data()[i] = replacement->data()[j];
        j++;
    }
}

std::vector<BYTE> CodingUtils::ToReversedBytesVector(ADDRESS_TYPE v) {
    std::vector<BYTE> bytesVector;

    bytesVector.reserve(sizeof(v));
    for (size_t i = 0; i < sizeof(v); i++) {
        bytesVector.push_back(v & 0xFF);
        v >>= 8;
    }

    return bytesVector;
}

UINT CodingUtils::HexStringToInt(std::string v) {
    return std::stoul(v, nullptr, 16);
}

long long CodingUtils::HexStringToLongLong(std::string v) {
    return std::stoull(v, nullptr, 16);
}

long long CodingUtils::HexWStringToLongLong(std::wstring v) {
    return std::stoull(v, nullptr, 16);
}

std::string CodingUtils::WStringToString(std::wstring wStr) {
    std::wstring wide(wStr);
    std::string str;
    std::transform(
        wide.begin(),
        wide.end(),
        std::back_inserter(str),
        [](wchar_t c) {
            return (char)c;
        }
    );
    return str;
}

std::wstring CodingUtils::GetThisExecutableFolderPath() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");

    return std::wstring(buffer).substr(0, pos);
}

UINT CodingUtils::GetTargetArchitecture() {
    if (sizeof(ADDRESS_TYPE) == 8) {
        return 0x64;
    }

    return 0x86;
}

ADDRESS_TYPE CodingUtils::ReverseBytes(UINT architecture, ADDRESS_TYPE v) {
    switch (architecture) {
    case 0x86:
        return _byteswap_ulong((DWORD32)v);
    case 0x64:
        return _byteswap_uint64((ULONG64)v);
    default:
        throw std::exception("Invalid architecture.");
        break;
    }
}
