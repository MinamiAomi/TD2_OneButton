#pragma once
#include <memory>

#include "Core/TextureResource.h"

#include "Math/MathUtils.h"

class Texture {
public:

    float GetWidth() const { return float(resource_.GetDesc().Width); }
    float GetHeight() const { return float(resource_.GetDesc().Height); }

private:
    TextureResource resource_;
};

class Sprite {
public:

    void SetTexture(const std::shared_ptr<Texture>& texture) { texture_ = texture; }
    void SetPosition(const Vector2& position) { position_ = position; }
    void SetRotate(float rotate) { rotate_ = rotate; }
    void SetScale(const Vector2& scale) { scale_ = scale; }
    void SetAnchor(const Vector2& anchor) { anchor_ = anchor; }
    void SetDrawOrder(uint8_t drawOrder) { drawOrder_ = drawOrder; }

private:
    std::shared_ptr<Texture> texture_;
    Vector2 position_;
    float rotate_;
    Vector2 scale_;
    Vector2 anchor_;
    uint8_t drawOrder_; // 小さいほど上に描画される レイヤーを表す値
};