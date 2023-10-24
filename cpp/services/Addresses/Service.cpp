#include "headers/pch.h"

#include "headers/utils/AddressUtils.h"
#include "headers/utils/CodingUtils.h"
#include "headers/services/Addresses/Service.h"
#include "headers/services/Console/Service.h"
#include "headers/services/Addresses/Addresses.h"
#include "headers/HackHandler.h"

using namespace AddressesNS;

Service::Service() {}

void Threads::AddressessThread(Service* s) {
    s->SetIsThreadRunning(TRUE);
    while (TRUE) {
        if (s->ShouldEndThread()) {
            s->SetIsThreadRunning(FALSE);
            return;
        }
        s->UpdateAddressesMap();
        Sleep(ADDRESSES_THREAD_TICKER_IN_MS);
    }
}

Service* Service::Init() {
    HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Threads::AddressessThread, this, 0, NULL);
    if (threadHandle) {
        CloseHandle(threadHandle);
    }
    else {
        throw std::runtime_error("Error while creating the addresses thread");
    }

    return this;
}

Service* Service::Reset() {
    this->addressesMap.clear();

    this->requestToEndThread = TRUE;
    while (this->GetIsThreadRunning()) {
        Sleep(20);
    }
    this->requestToEndThread = FALSE;
    
    return this;
}


BOOL Service::ShouldEndThread() {
    return this->requestToEndThread;
}

BOOL Service::GetIsThreadRunning() {
    return this->isThreadRunning;
}

Service* Service::SetIsThreadRunning(BOOL v) {
    this->isThreadRunning = v;
    return this;
}

Service* Service::UpdateAddressesMap() {
    /*
    for (auto const& [key, offsets] : OFFSETS) {
        auto address = AddressUtils::GetPointerPathFinalAddress(GetCurrentProcess(), (ADDRESS_TYPE)this->libhlHandle, offsets);
        this->addressesMap[key] = (ADDRESS_TYPE)address;
    }
    */

    return this;
}

BOOL Service::SetVariableValue(std::string variableName, ADDRESS_TYPE valueAddress) {
    if (!VARIABLE_TYPES.contains(variableName)) {
        throw std::runtime_error("Variable " + variableName + " does not exist.");
    }

    LPVOID variableAddress = (LPVOID)this->addressesMap[variableName];
    switch (VARIABLE_TYPES.at(variableName)) {
    case VariableType::UINT32:
        return this->writeMemory<UINT32>(variableAddress, *(UINT32*)valueAddress);
    case VariableType::INT32:
        return this->writeMemory<INT32>(variableAddress, *(INT32*)valueAddress);
    case VariableType::DOUBLE:
        return this->writeMemory<DOUBLE>(variableAddress, *(DOUBLE*)valueAddress);
    case VariableType::FLOAT:
        return this->writeMemory<FLOAT>(variableAddress, *(FLOAT*)valueAddress);
    case VariableType::BOOL:
        return this->writeMemory<BOOL>(variableAddress, *(BOOL*)valueAddress);
    default:
        throw std::runtime_error("Unsupported variable type.");
    }
}

std::map<std::string, ADDRESS_TYPE>* Service::GetAddressesMap() {
    return &this->addressesMap;
}