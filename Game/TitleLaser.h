#pragma once

#include <cstdint>

#include "Math/Transform.h"
#include "Graphics/ToonModel.h"

class TitleLaser {
public:
    void Initialize();
    void Update();
    bool EndAnimation() { return animationParameter_ >= animationLength_; }

private:
    Transform transform_;
    ToonModelInstance model_;
    uint32_t animationParameter_{};
    uint32_t animationLength_ = 120;
};