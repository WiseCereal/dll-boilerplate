#include "src/pch/pch.h"

#include "src/utils/AddressUtils.h"
#include "src/utils/CodingUtils.h"
#include "src/services/Addresses/Service.h"
#include "src/services/Console/Service.h"
#include "src/services/Addresses/Addresses.h"
#include "src/HackHandler.h"

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
        try {
            s->UpdateVariablesFile();
        }
        catch (std::exception e) {
            std::cout << "Error updating variables file" << e.what() << std::endl;
        }
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

Service* Service::UpdateVariablesFile() {
    std::string fileContents = "{";

    for (auto const& [variableName, value] : this->addressesMap) {
        fileContents = fileContents + "\"" + variableName + "\":";
        switch (VARIABLE_TYPES.at(variableName)) {
        case VariableType::UINT32: {
            fileContents = fileContents + std::to_string(this->Read<UINT32>(variableName)) + ",";
        } break;
        case VariableType::INT32: {
            fileContents = fileContents + std::to_string(this->Read<INT32>(variableName)) + ",";
        } break;
        case VariableType::DOUBLE: {
            fileContents = fileContents + std::to_string(this->Read<DOUBLE>(variableName)) + ",";
        } break;
        case VariableType::FLOAT: {
            fileContents = fileContents + std::to_string(this->Read<FLOAT>(variableName)) + ",";
        } break;
        case VariableType::BOOL: {
            fileContents = fileContents + std::to_string(this->Read<BOOL>(variableName)) + ",";
        } break;
        }
    }
    fileContents.pop_back();
    fileContents = fileContents + "}";

    std::ofstream file("variables_values.json");
    file << fileContents;
    file.close();

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