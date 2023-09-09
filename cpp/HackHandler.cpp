#include "headers/pch.h"

#include "headers/HackHandler.h"


using namespace HackHandlerNS;


Service::Service() {}

void Service::Init() {
    this->isReady = TRUE;
}

void Service::Reset() {
}

void Service::MainLoop() {
    this->checkEjectDLLKeysCombination();
}


void Service::MarkDLLToBeEjected() {
    this->shouldEjectDLL = TRUE;
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
