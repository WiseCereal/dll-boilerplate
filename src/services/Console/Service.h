#pragma once

#include "src/pch/pch.h"

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
