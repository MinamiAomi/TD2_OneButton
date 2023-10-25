#pragma once

#include "Engine/Scene/BaseScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Input/Input.h"
#include "Engine/Graphics/Sprite.h"
#include "Engine/Math/Transform.h"

#include "Graphics/ResourceManager.h"

#include "ClearLimit.h"

#include<memory>



class Clear : public BaseScene {
public:
    void OnInitialize() override;
    void OnUpdate() override;
    void OnFinalize() override;

    /// <summary>
    /// スプライトの基本初期処理
    /// </summary>
    void SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size);

    /// <summary>
    /// シーンチェンジに関する処理
    /// </summary>
    void SceneChange();

    /// <summary>
    /// スプライトを動かす
    /// </summary>
    void SpriteAnimation();

    /// <summary>
    /// イージングで使用するtの処理
    /// </summary>
    void EasingClamp(float& t, float increaseValue);

private:
    // キー入力
    Input* input_;

    // 経過時間
    uint32_t frame_;

#pragma region スプライト

    ResourceManager* resourceManager;
    // 中心座標
    const Vector3 center = { 270.0f, 360.0f, 0.0f };

    // 背景
    Sprite backGroundTex_;
    // YouWin
    Sprite youWinTex_;
    float youWinFadeInEasingT_ = 0.0f;
    float youWinFadeOutEasingT_ = 0.0f;
    bool isCompleteYouWinFadeIn_ = false;
    // GoodBye
    Sprite goodByeTex_;
    Vector2 goodByePosition_;
    float goodByeEasingT_ = 0.0f;
    // Bob
    Sprite bobTex_;
    float nameScaleEasingT_ = 0.0f;
    Sprite bobBodyTex_;
    float bossBodyFadeInEasingT_ = 0.0f;
    float bossBodySinMoveT_ = 0.0f;
    // Michael
    Sprite michaelTex_;
    Sprite michaelBodyTex_;
    // シーン遷移
    bool isPushSpace_ = false;
    Sprite blackBackGroundTex_;
    float blackScaleEasingT_;
    Sprite glayBackGroundTex_;
    float glayScaleEasingT_;

#pragma endregion

    // ボブかどうか（0=false,1=true)
    bool isBob_;

    // リミットスコアをインゲームから取得する
    uint32_t limitScore_ = 0;

    // リミットクラス
    std::unique_ptr<ClearLimit>clearLimit_ = nullptr;
};