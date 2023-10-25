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
        BOOL ShouldEndThread();
        BOOL GetIsThreadRunning();
        Service* SetIsThreadRunning(BOOL v);
        BOOL SetVariableValue(std::string variableName, ADDRESS_TYPE valueAddress);
        std::map<std::string, ADDRESS_TYPE>* GetAddressesMap();

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
