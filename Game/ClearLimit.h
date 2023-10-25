#pragma once

#include "Graphics/Sprite.h"

class ClearLimit {
public:
    void Initialize(uint32_t limitScore);
    void Update();

    // 桁調整
    void DigitAdjustment();
    // リミットのスコアを線形補間で表示する
    void ScoreAnimation();

private:

    struct Sprites {
        std::unique_ptr<Sprite> sprite;
        Vector2 position;
    };

    // 桁数
    static const int digitNum = 3;

    // 三桁分作成
    Sprites numSprite_[digitNum];

    Sprites meterSprite_;

    // 中心座標
    Vector2 center_ = { 215.0f,505.0f };

    // 各桁の数字
    int num_[3] = { 0,0,0 };

#pragma region リミットメートル
    // クリアシーンからリミットスコアを格納する
    uint32_t limit_ = 0;
    // 表示するリミットスコア
    uint32_t displayLimitMeter_ = 0;
    // 線形補間でリミットスコアとスコアバーを操作する
    float limitEasingt_ = 0.0f;
#pragma endregion

    // 一つの文字のサイズ
    const float texSize = 32.0f;

    const float texScale_ = 192;

    Vector2 mPos_ = { -9.0f,-36.0f };
    Vector2 mScale_ = { 80.0f,80.0f };
};
