#pragma once

#include "pch.h"

namespace AddressesNS {

    static const UINT ADDRESSES_THREAD_TICKER_IN_MS = 1000;

    class Service {
    private:
        std::map<std::string, ADDRESS_TYPE> addressesMap;

        BOOL requestToEndThread = FALSE;
        BOOL isThreadRunning = FALSE;
    public:
        Service();

        Service* Init();
        Service* Reset();
        Service* UpdateAddressesMap();
        Service* UpdateVariablesFile();
        BOOL ShouldEndThread();
        BOOL GetIsThreadRunning();
        Service* SetIsThreadRunning(BOOL v);
        BOOL SetVariableValue(std::string variableName, ADDRESS_TYPE valueAddress);
        std::map<std::string, ADDRESS_TYPE>* GetAddressesMap();

        template <typename T>
        T Read(std::string variableName) {
            T v = 0;
            ADDRESS_TYPE addr = this->addressesMap[variableName];
            if (ReadProcessMemory(GetCurrentProcess(), (LPVOID)addr, &v, sizeof(T), NULL)) {
                return v;
            }
            throw std::exception("Couldn't read memory");
        }

    private:
        template<typename T>
        BOOL writeMemory(LPVOID address, T value) {
            return WriteProcessMemory(GetCurrentProcess(), address, &value, sizeof(T), NULL);
        }
    };

    namespace Threads {
        void AddressessThread(Service* s);
    }

}
