#pragma once

#include "headers/pch.h"

#include "headers/services/Features/Feature.h"
#include "headers/features/TestFeature.h"

namespace FeaturesNS {

    class Service {
    private:
        TestFeatureNS::Feature testFeature;

        std::vector<Feature*> featuresVector;

    public:
        Service();

        Service* EnableFeature(std::string featureName);
        Service* DisableFeature(std::string featureName);
        Service* EnableAllFeatures();
        Service* DisableAllFeatures();
        Feature* GetFeature(std::string featureName);

        Service* SetTestBoostLevel(UINT level);

    private:
        Service* validateFeatures();
    };

}
