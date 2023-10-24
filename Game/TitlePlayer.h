#pragma once

#include <memory>
#include <vector>

#include "Math/Transform.h"
#include "Graphics/ToonModel.h"

class TitlePlayer {
public:
    void Initialize();
    void Update();

private:
    struct Part {
        enum Type {
            kHead,
            kBody,
            kLArm,
            kRArm,
            kLFoot,
            kRFoot,

            kNumParts
        };
        static const char* kModelNames[];

        Transform transform;
        ToonModelInstance model;
    };

    void InitializeParts();
    void UpdateParts();

    Transform baseTransform_;
    std::unique_ptr<Part> parts_[Part::kNumParts];
};