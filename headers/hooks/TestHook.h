#pragma once

#include "headers/pch.h"
#include "headers/services/Hooker/HookData.h"
#include "headers/features/TestFeature.h"


namespace HooksNS {

    namespace TestHook {

        namespace Features {
            static FeaturesNS::TestFeatureNS::Feature* TEST_FEATURE;
        }

        static const std::string NAME = "Test";
        static ADDRESS_TYPE HOOK_ADDRESS = 0;
        static ADDRESS_TYPE RETURN_ADDRESS = 0;

        class Data : public HookerNS::HookData {
        public:
            Data();
            ADDRESS_TYPE GetTrampolineRef();
            void InitFeatures();

            std::string GetName() {
                return NAME;
            }
        };

    }
}