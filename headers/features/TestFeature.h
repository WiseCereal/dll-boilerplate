#pragma once

#include "headers/pch.h"
#include "headers/services/Features/Feature.h"

namespace FeaturesNS {

    namespace TestFeatureNS {

        static const std::string NAME = "TestFeature";

        namespace Levels {
            static const UINT NORMAL = 1;
            static const UINT HIGHER = 2;
            static const UINT EXTREME = 3;
        }

        class Feature: public FeaturesNS::Feature {
        private:
            UINT boostLevel = Levels::NORMAL;

        public:
            std::string GetName();
            FeaturesNS::Feature* SetBoostLevel(UINT lvl);
            UINT GetBoostLevel();
        };
    }
}
