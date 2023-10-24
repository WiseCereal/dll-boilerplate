#include "headers/pch.h"

#include "headers/services/Addresses/Service.h"
#include "headers/services/Features/Feature.h"
#include "headers/services/Features/Service.h"
#include "headers/features/TestFeature.h"
#include "headers/exceptions/NotFoundException.h"

using namespace FeaturesNS;

Service::Service(AddressesNS::Service* addressesService) {
    this->addressesService = addressesService;

    this->initFeaturesMap();
    this->validateFeatures();
}

Service* Service::EnableFeature(std::string featureName) {
    this->GetFeature(featureName)->Enable();
    return this;
}

Service* Service::DisableFeature(std::string featureName) {
    this->GetFeature(featureName)->Disable();
    return this;
}

Service* Service::EnableAllFeatures() {
    for (auto const& [featureName, feature] : this->featuresMap) {
        feature->Enable();
    }
    return this;
}

Service* Service::DisableAllFeatures() {
    for (auto const& [featureName, feature] : this->featuresMap) {
        feature->Disable();
    }
    return this;
}

void Service::SetTestBoostLevel(UINT level) {
    if (level < TestFeatureNS::Levels::MIN_LEVEL || level > TestFeatureNS::Levels::MAX_LEVEL) {
        throw std::runtime_error("Invalid faster movement speed level");
    }

    auto feature = (TestFeatureNS::Feature*)this->GetFeature(TestFeatureNS::NAME);
    feature->SetBoostLevel(level);
}

Feature* Service::GetFeature(std::string featureName) {
    if (this->featuresMap.count(featureName)) {
        return this->featuresMap[featureName];
    }

    CustomExceptionsNS::NotFoundException ex;
    ex.setWhat("Feature " + featureName + " doesn't exist.");
    throw ex;
}

void Service::OnHackLoop() {
    for (auto const& [featureName, feature] : this->featuresMap) {
        if (!feature->IsEnabled()) {
            continue;
        }
        feature->OnHackLoop();
    }
}

Service* Service::validateFeatures() {
    std::vector<std::string> featureNames;
    for (auto const& [featureName, feature] : this->featuresMap) {
        for (auto& featureName : featureNames) {
            if (featureName == feature->GetName()) {
                throw std::runtime_error("Duplicated feature: " + featureName);
            }
        }
        featureNames.push_back(feature->GetName());
    }

    return this;
}

Service* Service::initFeaturesMap() {
    this->featuresMap[this->testFeature.GetName()] = &this->testFeature;

    for (auto const& [featureName, feature] : this->featuresMap) {
        feature->SetAddressesMap(this->addressesService->GetAddressesMap());
    }

    return this;
}