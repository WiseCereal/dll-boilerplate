#include "headers/pch.h"

#include "headers/services/FeaturesHandler/Feature.h"
#include "headers/services/FeaturesHandler/FeaturesHandler.h"
#include "headers/exceptions/NotFoundException.h"

using namespace FeaturesHandlerNS;

Service::Service() {
    this->featuresVector.push_back(&this->testFeature);

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
    for (auto feature : this->featuresVector) {
        feature->Enable();
    }
    return this;
}

Service* Service::DisableAllFeatures() {
    for (auto feature : this->featuresVector) {
        feature->Disable();
    }
    return this;
}

Service* Service::SetTestBoostLevel(UINT level) {
    auto feature = (FeaturesNS::TestFeatureNS::Feature*)this->GetFeature(FeaturesNS::TestFeatureNS::NAME);
    feature->SetBoostLevel(level);
    return this;
}


Feature* Service::GetFeature(std::string featureName) {
    for (auto feature : this->featuresVector) {
        if (feature->GetName() == featureName) {
            return feature;
        }
    }

    CustomExceptionsNS::NotFoundException ex;
    ex.setWhat("Feature " + featureName + " doesn't exist.");
    throw ex;
}

Service* Service::validateFeatures() {
    std::vector<std::string> featureNames;
    for (auto feature : this->featuresVector) {
        for (auto& featureName : featureNames) {
            if (featureName == feature->GetName()) {
                throw std::runtime_error("Duplicated feature: " + featureName);
            }
        }
        featureNames.push_back(feature->GetName());
    }

    return this;
}


