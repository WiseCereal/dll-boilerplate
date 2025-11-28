#pragma once

#include "src/pch/pch.h"

#include "src/services/Addresses/Service.h"
#include "src/services/Features/Feature.h"
#include "src/features/TestFeature.h"

namespace FeaturesNS {

    class Service {
    private:
        AddressesNS::Service* addressesService;

        TestFeatureNS::Feature testFeature;

        std::map<std::string, Feature*> featuresMap;

    public:
        Service(AddressesNS::Service* addressesService);

        Service* EnableFeature(std::string featureName);
        Service* DisableFeature(std::string featureName);
        Service* EnableAllFeatures();
        Service* DisableAllFeatures();
        Feature* GetFeature(std::string featureName);
        void OnHackLoop();

        void SetTestBoostLevel(UINT level);

    private:
        Service* initFeaturesMap();
        Service* validateFeatures();
    };

}
