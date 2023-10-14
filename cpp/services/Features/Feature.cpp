#include "headers/pch.h"
#include "headers/services/Features/Feature.h"

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
