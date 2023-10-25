#include "ClearLimit.h"

void ClearLimit::Initialize(uint32_t limitScore, bool isBob) {

    resourceManager = ResourceManager::GetInstance();

    // 画像の初期化
    for (int i = 0; i < digitNum; i++) {
        numSprite_[i].sprite = std::make_unique<Sprite>();
        numSprite_[i].sprite->SetTexture(resourceManager->FindTexture("number"));

        numSprite_[i].sprite->SetAnchor({ 0.5f,0.5f });
        numSprite_[i].sprite->SetScale({ texScale_, texScale_ });
        numSprite_[i].sprite->SetIsActive(false);
        numSprite_[i].sprite->SetColor({ 0.0f,0.0f,0.0f,1.0f });
    }

    meterSprite_.sprite = std::make_unique<Sprite>();
    meterSprite_.sprite->SetTexture(resourceManager->FindTexture("limit"));
    meterSprite_.sprite->SetAnchor({ 0.5f,0.5f });
    meterSprite_.sprite->SetScale(mScale_);
    meterSprite_.sprite->SetTexcoordRect({ 0.0f,0.0f }, { 150.0f,200.0f });
    meterSprite_.sprite->SetIsActive(false);
    meterSprite_.sprite->SetColor({ 0.0f,0.0f,0.0f,1.0f });

    SpriteInitialize(clearLimitTex_, "clearLimit", { 150.0f, 32.0f });
    clearLimitTex_.SetPosition({ 100.0f, 570.0f });
    clearLimitTex_.SetIsActive(false);

    Vector2 baseMPosition[3] = {
        { 120.0f, 300.0f },
        { 270.0f, 300.0f },
        { 420.0f, 300.0f }
    };
    for (size_t i = 0; i < kMCount_; i++) {
        SpriteInitialize(mTex_[i], "m", { 150.0f, 150.0f });
        SpriteInitialize(mFrameTex_[i], "mFrame", { 150.0f, 150.0f });
        SpriteInitialize(mScaleTex_[i], "mFrame", { 150.0f, 150.0f });
        mTex_[i].SetPosition(baseMPosition[i]);
        mFrameTex_[i].SetPosition(baseMPosition[i]);
        mScaleTex_[i].SetPosition(baseMPosition[i]);
        mTex_[i].SetIsActive(false);
        mFrameTex_[i].SetIsActive(false);
        mScaleTex_[i].SetIsActive(false);
    }
    SpriteInitialize(clearTex_, "clear", { 200.0f, 64.0f });
    SpriteInitialize(tenLimitMeterTex_, "10LimitMeter", { 200.0f, 64.0f });
    SpriteInitialize(thirtyLimitMeterTex_, "30LimitMeter", { 200.0f, 64.0f });
    clearTex_.SetPosition({ baseMPosition[0].x, baseMPosition[0].y - 75.0f });
    clearTex_.SetScale({ 80.0f, 25.6f });
    clearTex_.SetIsActive(false);
    tenLimitMeterTex_.SetPosition({ baseMPosition[1].x, baseMPosition[1].y - 75.0f });
    tenLimitMeterTex_.SetScale({ 80.0f, 25.6f });
    tenLimitMeterTex_.SetIsActive(false);
    thirtyLimitMeterTex_.SetPosition({ baseMPosition[2].x, baseMPosition[2].y - 75.0f });
    thirtyLimitMeterTex_.SetScale({ 80.0f, 25.6f });
    thirtyLimitMeterTex_.SetIsActive(false);

    limit_ = limitScore;
    isBob_ = isBob;
}

void ClearLimit::Update(uint32_t frame) {

    // Updateが呼び出された時点でスプライトをActiveにする
    if (!isDisplay_) {
        for (int i = 0; i < digitNum; i++) {
            numSprite_[i].sprite->SetIsActive(true);
        }
        meterSprite_.sprite->SetIsActive(true);
        clearLimitTex_.SetIsActive(true);
        for (size_t i = 0; i < kMCount_; i++) {
            mFrameTex_[i].SetIsActive(true);
        }
        clearTex_.SetIsActive(true);
        tenLimitMeterTex_.SetIsActive(true);
        thirtyLimitMeterTex_.SetIsActive(true);
        isDisplay_ = true;
    }

    // スコアの表示に動きをつける
    ScoreAnimation(frame);
    // 桁の調整を行う
    DigitAdjustment();
}

void ClearLimit::ScoreAnimation(uint32_t frame) {

    auto easeOutCirc = [](float t) { return std::sqrtf(1 - std::powf(t - 1, 2)); };

    EasingClamp(limitEasingt_, 0.02f);

    // 数値をイージングで増加し表示
    displayLimitMeter_ = uint32_t(Math::Lerp(easeOutCirc(limitEasingt_), 0, float(limit_)));

    // フェードイン
    for (int i = 0; i < digitNum; i++) {
        numSprite_[i].sprite->SetColor({ 0.0f,0.0f,0.0f,easeOutCirc(limitEasingt_) });
    }
    meterSprite_.sprite->SetColor({ 0.0f,0.0f,0.0f,easeOutCirc(limitEasingt_) });
    clearLimitTex_.SetColor({ 1.0f,1.0f,1.0f,easeOutCirc(limitEasingt_) });
    for (size_t i = 0; i < kMCount_; i++) {
        mFrameTex_[i].SetColor({ 1.0f,1.0f,1.0f,easeOutCirc(limitEasingt_) });
    }
    clearTex_.SetColor({ 1.0f,1.0f,1.0f,easeOutCirc(limitEasingt_) });
    tenLimitMeterTex_.SetColor({ 1.0f,1.0f,1.0f,easeOutCirc(limitEasingt_) });
    thirtyLimitMeterTex_.SetColor({ 1.0f,1.0f,1.0f,easeOutCirc(limitEasingt_) });

    // mの取得処理
    if (frame > 380 && limit_ >= 30) {
        mTex_[2].SetIsActive(true);
        ScaleFadeOut(scaleEasingT_[2], mScaleTex_[2]);
    }
    if (frame > 340 && limit_ >= 10) {
        mTex_[1].SetIsActive(true);
        ScaleFadeOut(scaleEasingT_[1], mScaleTex_[1]);
    }
    if (frame > 300) {
        mTex_[0].SetIsActive(true);
        ScaleFadeOut(scaleEasingT_[0], mScaleTex_[0]);
    }
}



void ClearLimit::SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size) {
    sprite.SetTexture(resourceManager->FindTexture(textureName));
    sprite.SetAnchor({ 0.5f,0.5f });
    sprite.SetTexcoordRect({ 0.0f,0.0f }, size);
    sprite.SetScale(size);
}

void ClearLimit::EasingClamp(float& t, float increaseValue) {
    if (t < 1.0f) {
        t += increaseValue;
    }
    if (t > 1.0f) {
        t = 1.0f;
    }
}

void ClearLimit::ScaleFadeOut(float& t, Sprite& sprite) {
    auto easeOutCirc = [](float t1) { return std::sqrtf(1 - std::powf(t1 - 1, 2)); };
    EasingClamp(t, 0.02f);
    if (t != 1.0f) {
        sprite.SetIsActive(true);
    }
    else {
        sprite.SetIsActive(false);
    }
    sprite.SetScale(Vector2::Lerp(easeOutCirc(t), { 150.0f, 150.0f }, { 200.0f, 200.0f }));
    sprite.SetColor({ 1.0f,1.0f,1.0f,Math::Lerp(easeOutCirc(t), 1.0f, 0.0f) });
}

void ClearLimit::DigitAdjustment() {

    if (displayLimitMeter_ != 0) {
        // 一桁目取得
        num_[0] = displayLimitMeter_ % 10;
        // 二桁目の取得
        num_[1] = displayLimitMeter_ - num_[0];

        uint32_t count = 0;
        while (true) {
            count++;
            num_[1] -= 10;
            // 二桁目の数字を作成
            if (num_[1] == 0) {

                // 三桁目がある場合
                if (count >= 10) {
                    // 二桁目の数字を入力
                    num_[1] = count % 10;
                    numSprite_[1].sprite->SetIsActive(true);
                    // 値を代入
                    num_[2] = count;
                    // 初期化
                    count = 0;
                    while (true) {
                        count++;
                        num_[2] -= 10;
                        // 三桁目の数字を取得
                        if (num_[2] == 0) {
                            num_[2] = count;
                            numSprite_[2].sprite->SetIsActive(true);

                            numSprite_[0].position = center_;
                            numSprite_[0].position.x += texScale_ / 2.0f;

                            numSprite_[1].position = center_;

                            numSprite_[0].position = center_;
                            numSprite_[0].position.x -= texScale_ / 2.0f;

                            break;
                        }
                    }
                }
                else {// ない場合
                    num_[1] = count;
                    numSprite_[1].sprite->SetIsActive(true);
                    numSprite_[2].sprite->SetIsActive(false);

                    // 座標処理
                    numSprite_[0].position = center_;
                    numSprite_[0].position.x += texScale_ / 4.0f;

                    numSprite_[1].position = center_;
                    numSprite_[1].position.x -= texScale_ / 4.0f;

                    break;
                }


            }// 以下一桁の場合
            else if (num_[1] < 0.0f) {

                // 数字を中心点に
                numSprite_[0].position = center_;

                numSprite_[1].sprite->SetIsActive(false);
                numSprite_[2].sprite->SetIsActive(false);


                break;
            }
        }

    }
    else {
        num_[0] = 0;
        // 数字を中心点に
        numSprite_[0].position = center_;

        numSprite_[1].sprite->SetIsActive(false);
        numSprite_[2].sprite->SetIsActive(false);

    }

    for (int i = 0; i < digitNum; i++) {
        numSprite_[i].sprite->SetPosition(numSprite_[i].position);
        numSprite_[i].sprite->SetTexcoordRect({ texSize * num_[i],0.0f }, { texSize,texSize });
    }

    // 座標設定
    meterSprite_.position = numSprite_[0].position;
    meterSprite_.position.x = numSprite_[0].position.x + texScale_ / 2.0f;
    meterSprite_.position += mPos_;
    meterSprite_.sprite->SetPosition(meterSprite_.position);
}
