#pragma once

#include "pch.h"
#include "src/utils/CodingUtils.h"

namespace FeaturesNS {

    class Feature {
    protected:
        BOOL enabled = FALSE;
        std::map<std::string, ADDRESS_TYPE>* addressesMap = {};
    public:
        virtual ~Feature() {}
        virtual std::string GetName() = 0;
        virtual void OnHackLoop() = 0;

        Feature() {}

        Feature* Enable();
        Feature* Disable();
        BOOL IsEnabled();
        Feature* SetAddressesMap(std::map<std::string, ADDRESS_TYPE>* addressesMap);
    };

}
