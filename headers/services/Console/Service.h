#pragma once

#include "headers/pch.h"

namespace ConsoleNS {

    class Service {
    private:
        FILE* file = NULL;

    public:
        Service();
        BOOL OpenConsole();
        BOOL CloseConsole();
    };

}
