#include "headers/pch.h"

#include "headers/services/Features/Feature.h"
#include "headers/features/TestFeature.h"
#include "headers/services/Features/Service.h"
#include "headers/exceptions/NotFoundException.h"

using namespace FeaturesNS;

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

void Service::SetTestBoostLevel(UINT level) {
    if (level < TestFeatureNS::Levels::MIN_LEVEL || level > TestFeatureNS::Levels::MAX_LEVEL) {
        throw std::runtime_error("Invalid faster movement speed level");
    }

    auto feature = (TestFeatureNS::Feature*)this->GetFeature(TestFeatureNS::NAME);
    feature->SetBoostLevel(level);
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

void Service::OnHackLoop() {
    for (auto feature : this->featuresVector) {
        if (feature->IsEnabled()) {
            feature->OnHackLoop();
        }
    }
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


