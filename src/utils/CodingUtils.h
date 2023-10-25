#pragma once

#include "pch.h"

namespace CodingUtils {

    bool AreStringsEqual(std::string str1, std::string str2);

    bool AreWStringsEqual(std::wstring str1, std::wstring str2);

    bool StringContainsString(std::string haystack, std::string needle);

    bool WStringContainsWString(std::wstring haystack, std::wstring needle);

    void ByteArrayReplace(size_t index, std::vector<BYTE>* replacement, std::vector<BYTE>* byteArray);

    std::vector<BYTE> ToReversedBytesVector(ADDRESS_TYPE v);

    UINT HexStringToInt(std::string v);

    long long HexStringToLongLong(std::string v);

    long long HexWStringToLongLong(std::wstring v);

    std::string WStringToString(std::wstring wStr);

    std::wstring GetThisExecutableFolderPath();

    UINT GetTargetArchitecture();

    ADDRESS_TYPE ReverseBytes(UINT architecture, ADDRESS_TYPE v);

}
