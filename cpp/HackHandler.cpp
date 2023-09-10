#include "headers/pch.h"

#include "headers/HackHandler.h"

#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/services/Hooker/Hooker.h"


using namespace HackHandlerNS;


Service::Service(
    HookerNS::Service* hookerService,
    FeaturesHandlerNS::Service* featuresHandler
) {
    this->hookerService = hookerService;
    this->featuresHandler = featuresHandler;
}

void Service::Init() {
    this->hookerService->InitHooks();

    this->isReady = TRUE;
}

void Service::Reset() {
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

void Service::checkEjectDLLKeysCombination() {
    for (auto key : EJECT_DLL_KEYS_COMBINATION)
        if (!(GetAsyncKeyState(key) & 0x8000)) // coding utils, IsKeyPressed :)
            return;

    this->MarkDLLToBeEjected();
}
