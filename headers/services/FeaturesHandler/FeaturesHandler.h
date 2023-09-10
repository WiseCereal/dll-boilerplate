#pragma once

#include "headers/pch.h"

#include "headers/services/FeaturesHandler/Feature.h"
#include "headers/features/TestFeature.h"

namespace FeaturesHandlerNS {

    class Service {
    private:
        FeaturesNS::TestFeatureNS::Feature testFeature;

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
