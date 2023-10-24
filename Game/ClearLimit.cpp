#include "ClearLimit.h"
#include "Graphics/ResourceManager.h"

void ClearLimit::Initialize(uint32_t limitScore) {

    const char textureName[] = "number";
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    Vector2 texScale = { texScale_,texScale_ };

    // 画像の初期化
    for (int i = 0; i < digitNum; i++) {
        numSprite_[i].sprite = std::make_unique<Sprite>();
        numSprite_[i].sprite->SetTexture(resourceManager->FindTexture(textureName));

        numSprite_[i].sprite->SetAnchor({ 0.5f,0.5f });
        numSprite_[i].sprite->SetScale(texScale);
        numSprite_[i].sprite->SetIsActive(true);
        numSprite_[i].sprite->SetColor({ 0.0f,0.0f,0.0f,1.0f });
    }

    const char meterName[] = "limit";

    meterSprite_.sprite = std::make_unique<Sprite>();
    meterSprite_.sprite->SetTexture(resourceManager->FindTexture(meterName));
    meterSprite_.sprite->SetAnchor({ 0.5f,0.5f });
    meterSprite_.sprite->SetScale(mScale_);
    meterSprite_.sprite->SetTexcoordRect({ 0.0f,0.0f }, { 150.0f,200.0f });
    meterSprite_.sprite->SetIsActive(true);
    meterSprite_.sprite->SetColor({ 0.0f,0.0f,0.0f,1.0f });

    limit_ = limitScore;
}

void ClearLimit::Update() {
    // スコアの表示に動きをつける
    ScoreAnimation();
    // 桁の調整を行う
    DigitAdjustment();
}

void ClearLimit::ScoreAnimation() {

    // tの値を増やしクランプする
    if (limitEasingt_ < 1.0f) {
        limitEasingt_ += 0.01f;
    }
    else if (limitEasingt_ > 1.0f) {
        limitEasingt_ = 1.0f;
    }

    displayLimitMeter_ = uint32_t(Math::Lerp(limitEasingt_, 0, float(limit_)));
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
