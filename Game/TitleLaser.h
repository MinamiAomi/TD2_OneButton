#pragma once

#include <cstdint>
#include <memory>

#include "Math/Transform.h"
#include "Graphics/ToonModel.h"
#include "Graphics/Sprite.h"

class TitleLaser {
public:
    void Initialize();
    void Update();
    bool EndAnimation() { return animationParameter_ >= 1.0f; }

private:
    // アニメーションの長さ
    static constexpr uint32_t kAnimationDuration = 30;
    
    Transform transform_;
    ToonModelInstance model_;
    Sprite explosionSprite_;

    float animationParameter_;
    float startWidth_;
    float endWidth_;
};