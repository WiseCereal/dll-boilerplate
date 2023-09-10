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

std::string CodingUtils::ByteArrayToHexString(BYTE* data, size_t len) {
    std::stringstream ss;

    ss << std::hex;
    for (size_t i = 0; i < len; ++i) {
        ss << std::setw(2) << std::setfill('0') << (int)data[i];
    }

    return ss.str();
}

void CodingUtils::ByteArrayReplace(size_t index, std::string replacement, BYTE* byteArray) {
    std::string strByte = "";
    int i = 0;
    for (char& c : replacement) {
        strByte.append(std::string(1, c));
        i++;
        if ((!(i % 2)) && i > 0) {
            byteArray[index] = (BYTE)stoul(strByte, nullptr, 16);
            index++;
            strByte = "";
        }
    }
}

std::string CodingUtils::LeftZeroPad(std::string str, UINT desiredLength) {
    if (str.length() >= desiredLength) {
        return str;
    }

    while (str.length() < desiredLength) {
        str.insert(0, "0");
    }

    return str;
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