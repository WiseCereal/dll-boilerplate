#pragma once

#include "headers/pch.h"
#include "headers/utils/CodingUtils.h"

namespace FeaturesHandlerNS {

    class Feature {
    protected:
        BOOL enabled = FALSE;
    public:
        virtual ~Feature() {}
        virtual std::string GetName() = 0;

        Feature() {}

        Feature* Enable();
        Feature* Disable();
        BOOL IsEnabled();
    };

}
