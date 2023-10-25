#pragma once

#include <cstdint>
#include <memory>

#include "Math/Transform.h"
#include "Graphics/ToonModel.h"

class TitleLaser {
public:
    void Initialize();
    void Update();
    bool EndAnimation() { return animationParameter_ >= 1.0f; }

private:
    // アニメーションの長さ
    static constexpr uint32_t kAnimationDuration = 20;
    
    Transform transform_;
    ToonModelInstance model_;
    Transform explosionTransform_;
    ToonModelInstance explosionModel_;

    float animationParameter_;
    float startWidth_;
    float endWidth_;
};