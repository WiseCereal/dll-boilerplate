#pragma once

#include "headers/pch.h"
#include "headers/utils/RegistersUtils.h"
#include "headers/features/TestFeature.h"
#include "headers/services/Hooker/HookData.h"


namespace HooksNS {

    namespace TestHook {

        namespace Features {
            static FeaturesNS::TestFeatureNS::Feature* TEST_FEATURE;
        }

        static const std::string NAME = "Test";

        class Data : public HookerNS::HookData {
        public:
            Data();
            std::vector<BYTE>* GetTrampolineBytes(UINT jmpSkeletonSize);
            void InitFeatures();
            BOOL DirectReadWithOffset();
            RegistersUtils::Register GetRegisterForSafeJump();
            void PrepareTrampolineBytes(std::vector<BYTE> trampolineSkeleton, UINT jmpSkeletonSize);

            std::string GetName() {
                return NAME;
            }
        };

    }
}
