#include "pch.h"

#include "src/services/Addresses/Service.h"
#include "src/services/Features/Feature.h"

using namespace FeaturesNS;

Feature* Feature::Enable() {
    this->enabled = TRUE;
    return this;
}

Feature* Feature::Disable() {
    this->enabled = FALSE;
    return this;
}

BOOL Feature::IsEnabled() {
    return this->enabled;
}

Feature* Feature::SetAddressesMap(std::map<std::string, ADDRESS_TYPE>* addressesMap) {
    this->addressesMap = addressesMap;
    return this;
}
