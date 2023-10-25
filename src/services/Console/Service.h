#pragma once

#include "pch.h"

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
