#pragma once

#include "Engine/Scene/BaseScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Input/Input.h"
#include "Engine/Graphics/Sprite.h"
#include "Engine/Math/Transform.h"

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

private:
    // キー入力
    Input* input_;

#pragma region スプライトとその座標

    // 中心座標
    const Vector3 center = { 270.0f, 360.0f, 0.0f };

    // 背景
    Sprite backGroundTex_;
    Transform backGroundTransform_;
    // YouWin
    Sprite youWinTex_;
    Transform youWinTransform_;
    // GoodBye
    Sprite goodByeTex_;
    Transform goodByeTransform_;
    // Bob
    Sprite bobTex_;
    Transform bobTransform_;
    // Michael
    Sprite michaelTex_;
    Transform michaelTransform_;
    // スコアバーの線
    Sprite scoreBarLineTex_;
    Transform scoreBarLineTransform_;
    // スコアバーの円
    Sprite scoreBarCircleTex_;
    Transform scoreBarCircleTransform_;
    // 仮の下端絵
    Sprite tmpBottomTex_;
    Transform tmpBottomTransform_;

#pragma endregion

    // ボブかどうか（0=false,1=true)
    bool isBob_;

    // リミットスコアをインゲームから取得する
    uint32_t limitScore_ = 0;

    // リミットクラス
    std::unique_ptr<ClearLimit>clearLimit_ = nullptr;
};