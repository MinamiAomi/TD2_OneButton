#pragma once

#include "Graphics/Sprite.h"
#include "Graphics/ResourceManager.h"

class ClearLimit {
public:
    void Initialize(uint32_t limitScore, bool isBob);
    void Update(uint32_t frame);

    // 桁調整
    void DigitAdjustment();
    // リミットのスコアを線形補間で表示する
    void ScoreAnimation(uint32_t frame);

    /// <summary>
    /// スプライトの基本初期処理
    /// </summary>
    void SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size);

    /// <summary>
    /// イージングで使用するtの処理
    /// </summary>
    void EasingClamp(float& t, float increaseValue);

    /// <summary>
    /// 拡大しながらフェードアウト
    /// </summary>
    void ScaleFadeOut(float& t, Sprite& sprite);

private:

    ResourceManager* resourceManager;

    struct Sprites {
        std::unique_ptr<Sprite> sprite;
        Vector2 position;
    };

    // 表示したかフラグ
    bool isDisplay_ = false;

    // 討伐した相手がbobかどうか取得
    bool isBob_;

    // 桁数
    static const int digitNum = 3;

#pragma region スプライト

    // 三桁分作成
    Sprites numSprite_[digitNum];
    // リミットメートル
    Sprites meterSprite_;
    // クリアリミット
    Sprite clearLimitTex_;
    // クリア星
    static const uint32_t kMCount_ = 3;
    Sprite mTex_[kMCount_];
    Sprite mFrameTex_[kMCount_];
    Sprite mScaleTex_[kMCount_];
    float scaleEasingT_[kMCount_] = { 0.0f, 0.0f, 0.0f };
    // ノルマ
    Sprite clearTex_;
    Sprite tenLimitMeterTex_;
    Sprite thirtyLimitMeterTex_;

#pragma endregion

    // 中心座標
    Vector2 center_ = { 180.0f,480.0f };

    // 各桁の数字
    int num_[3] = { 0,0,0 };

#pragma region リミットメートル
    // クリアシーンからリミットスコアを格納する
    uint32_t limit_ = 0;
    // 表示するリミットスコア
    uint32_t displayLimitMeter_ = 0;
    // 線形補間でリミットスコアを操作する
    float limitEasingt_ = 0.0f;
#pragma endregion

    // 一つの文字のサイズ
    const float texSize = 32.0f;

    const float texScale_ = 192;

    Vector2 mPos_ = { -9.0f,-36.0f };
    Vector2 mScale_ = { 80.0f,80.0f };
};
