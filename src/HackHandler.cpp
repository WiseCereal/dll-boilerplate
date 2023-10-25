#include "pch.h"

#include "src/HackHandler.h"
#include "src/services/Features/Service.h"
#include "src/services/Addresses/Service.h"
#include "src/services/Hooker/Service.h"


using namespace HackHandlerNS;

Service::Service(
    HookerNS::Service* hookerService,
    FeaturesNS::Service* featuresService,
    AddressesNS::Service* addressesService
) {
    this->hookerService = hookerService;
    this->featuresService = featuresService;
    this->addressesService = addressesService;
}

void __stdcall InitThread(Service* handler) {
    handler->GetHookerService()->InitHooks(
        [handler]() {
            handler->GetAddressesService()->Init();
            handler->MarkAsReady();
        }
    );
}
void Service::Init() {
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitThread, this, 0, NULL);
}

void Service::Reset() {
    this->hookerService->Reset();
    this->addressesService->Reset();
}

void Service::MainLoop() {
    this->checkEjectDLLKeysCombination();

    if (this->shouldEjectDLL) {
        return;
    }

    this->featuresService->OnHackLoop();
}


BOOL Service::MarkDLLToBeEjected() {
    this->shouldEjectDLL = TRUE;
    return TRUE;
}


BOOL Service::ShouldDLLBeEjected() {
    return this->shouldEjectDLL;
}

BOOL Service::IsReady() {
    return this->isReady;
}

void Service::MarkAsReady() {
    std::cout << "Everything is ready" << std::endl;
    this->isReady = TRUE;
}

HookerNS::Service* Service::GetHookerService() {
    return this->hookerService;
}
FeaturesNS::Service* Service::GetFeaturesService() {
    return this->featuresService;
}
AddressesNS::Service* Service::GetAddressesService() {
    return this->addressesService;
}

void Service::checkEjectDLLKeysCombination() {
    for (auto key : EJECT_DLL_KEYS_COMBINATION)
        if (!(GetAsyncKeyState(key) & 0x8000)) // coding utils, IsKeyPressed :)
            return;

    this->Reset();
    this->MarkDLLToBeEjected();
}
