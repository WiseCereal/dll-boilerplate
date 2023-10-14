#include "headers/pch.h"
#include "headers/features/TestFeature.h"

using namespace FeaturesNS::TestFeatureNS;

std::string Feature::GetName() {
    return NAME;
}

FeaturesNS::Feature* Feature::SetBoostLevel(UINT level) {
    if (level < Levels::NORMAL || level > Levels::EXTREME) {
        throw std::runtime_error("Invalid test level");
    }

    this->boostLevel = level;
    return this;
}

UINT Feature::GetBoostLevel() {
    return this->boostLevel;
}
