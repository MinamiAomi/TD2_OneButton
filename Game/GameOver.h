#pragma once

#include "Engine/Scene/BaseScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Input/Input.h"
#include "Engine/Graphics/Sprite.h"

#include "Graphics/ResourceManager.h"

class GameOver : public BaseScene {

public:
    void OnInitialize() override;
    void OnUpdate() override;
    void OnFinalize() override;

    /// <summary>
    /// スプライトの基本初期処理
    /// </summary>
    void SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size);

    /// <summary>
    /// イージングで使用するtの処理
    /// </summary>
    void EasingClamp(float& t, float increaseValue);

private:
    Input* input_;
    ResourceManager* resourceManager;

    // 中心座標
    const Vector3 center = { 270.0f, 360.0f, 0.0f };
    // 経過時間
    uint32_t frame_ = 0;

#pragma region スプライト

    // 背景
    Sprite backGroundTex_;
    // YouLose
    Sprite youLoseTex_;
    float youLoseFadeInEasingT_ = 0.0f;
    float youLoseFadeOutEasingT_ = 0.0f;
    bool isCompleteYouLoseFadeIn_ = false;

#pragma endregion
};