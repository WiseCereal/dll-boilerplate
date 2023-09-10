#pragma once

#include "headers/pch.h"

namespace CodingUtils {

    bool AreStringsEqual(std::string str1, std::string str2);

    bool AreWStringsEqual(std::wstring str1, std::wstring str2);

    bool StringContainsString(std::string haystack, std::string needle);

    bool WStringContainsWString(std::wstring haystack, std::wstring needle);

    std::string ByteArrayToHexString(BYTE* data, size_t len);

    void ByteArrayReplace(size_t index, std::string replacement, BYTE* byteArray);

    std::string LeftZeroPad(std::string str, UINT desiredLength);

    UINT HexStringToInt(std::string v);

    long long HexStringToLongLong(std::string v);

    long long HexWStringToLongLong(std::wstring v);

    std::string WStringToString(std::wstring wStr);

    std::wstring GetThisExecutableFolderPath();

    template<typename T>
    std::string ScalarToHexString(T v) {
        std::ostringstream stream;
        stream << std::hex << v;
        return stream.str();
    }

    template<typename T>
    T CastLPVOID(LPVOID v) {
        return *((T*)(&v));
    }

}
