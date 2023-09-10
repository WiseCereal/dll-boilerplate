#pragma once

#include "headers/pch.h"

namespace ConsoleHandlerNS {

    class Handler {
    private:
        FILE* file = NULL;

    public:
        Handler();
        BOOL OpenConsole();
        BOOL CloseConsole();
    };

}
