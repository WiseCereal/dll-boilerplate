#pragma once

#include "headers/pch.h"

namespace ConsoleHandlerNS {


    class Interface {
    public:
        virtual ~Interface() {}

        virtual BOOL OpenConsole() = 0;
        virtual BOOL CloseConsole() = 0;
    };


    class Handler : public Interface {
    private:
        FILE* file = NULL;

    public:
        Handler();
        BOOL OpenConsole();
        BOOL CloseConsole();
    };

}
