#include "headers/pch.h"

#include "headers/HackHandler.h"

#include "headers/services/Features/Service.h"
#include "headers/services/Hooker/Service.h"


using namespace HackHandlerNS;


Service::Service(
    HookerNS::Service* hookerService,
    FeaturesNS::Service* featuresService
) {
    this->hookerService = hookerService;
    this->featuresService = featuresService;
}

void InitThread(Service* handler) {
    handler->GetHookerService()->InitHooks(
        [handler]() {
            handler->MarkAsReady();
        }
    );
}
void Service::Init() {
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitThread, this, 0, NULL);
}


void Service::Reset() {
    this->hookerService->Reset();
}

void Service::MainLoop() {
    this->checkEjectDLLKeysCombination();
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
    std::cout << "Everything ready" << std::endl;
    this->isReady = TRUE;
}

HookerNS::Service* Service::GetHookerService() {
    return this->hookerService;
}


void Service::checkEjectDLLKeysCombination() {
    for (auto key : EJECT_DLL_KEYS_COMBINATION)
        if (!(GetAsyncKeyState(key) & 0x8000)) // coding utils, IsKeyPressed :)
            return;

    this->Reset();
    this->MarkDLLToBeEjected();
}
