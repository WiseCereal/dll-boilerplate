#pragma once

#include "headers/pch.h"
#include "headers/services/Features/Feature.h"

namespace FeaturesNS {

    namespace TestFeatureNS {

        static const std::string NAME = "TestFeature";

        namespace Levels {
            static const UINT MIN_LEVEL = 1;
            static const UINT MAX_LEVEL = 5;
        }

        class Feature: public FeaturesNS::Feature {
        private:
            UINT boostLevel = Levels::MIN_LEVEL;

        public:
            void OnHackLoop();
            std::string GetName();
            FeaturesNS::Feature* SetBoostLevel(UINT lvl);
            UINT GetBoostLevel();
        };
    }
}
