#include "src/pch/pch.h"
#include "src/features/TestFeature.h"

using namespace FeaturesNS::TestFeatureNS;

std::string Feature::GetName() {
    return NAME;
}

FeaturesNS::Feature* Feature::SetBoostLevel(UINT level) {
    this->boostLevel = level;
    return this;
}

UINT Feature::GetBoostLevel() {
    return this->boostLevel;
}

void Feature::OnHackLoop() {}