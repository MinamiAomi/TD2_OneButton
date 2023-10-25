#include "GlobalVariables.h"
#include "GameOver.h"
#include "StageSerect.h"
#include "ImGame.h"
#include "Audio/Audio.h"

void GameOver::OnInitialize() {
    input_ = Input::GetInstance();

#pragma region 画像初期化

    resourceManager = ResourceManager::GetInstance();
    // 背景
    SpriteInitialize(backGroundTex_, "clearBackGround", { 540.0f, 720.0f });
    backGroundTex_.SetPosition({ center.x, center.y });
    // YouWin
    SpriteInitialize(youLoseTex_, "youLose", { 400.0f, 64.0f });
    youLoseTex_.SetPosition({ center.x, center.y });


#pragma endregion

    Audio* audio = Audio::GetInstance();
    bgmPlayHandle_ = audio->SoundPlayLoopStart(resourceManager->FindSound("ResultBGM"));
    audio->SetValume(bgmPlayHandle_, 0.2f);
}

void GameOver::OnUpdate() {

    frame_++;

    auto easeOutCirc = [](float t) { return std::sqrtf(1 - std::powf(t - 1, 2)); };
    auto easeInSine = [](float t) { return 1 - std::cosf((t * Math::Pi) / 2); };

    // YouLose
    if (!isCompleteYouLoseFadeIn_) {
        EasingClamp(youLoseFadeInEasingT_, 0.02f);
        if (youLoseFadeInEasingT_ == 1.0f) {
            isCompleteYouLoseFadeIn_ = true;
        }
        youLoseTex_.SetColor({ 1.0f, 1.0f, 1.0f, easeOutCirc(youLoseFadeInEasingT_) });
    }
    else if (frame_ > 40)
    {
        EasingClamp(youLoseFadeOutEasingT_, 0.02f);
        youLoseFadeInEasingT_ = 1.0f - youLoseFadeOutEasingT_;
        youLoseTex_.SetColor({ 1.0f, 1.0f, 1.0f, easeInSine(youLoseFadeInEasingT_) });
        if (youLoseFadeOutEasingT_ == 1.0f) {
            // インスタンス取得
            SceneManager* sceneManager = SceneManager::GetInstance();
            // シーン設定
            sceneManager->ChangeScene<StageSerect>();
        }
    }
}

void GameOver::OnFinalize() {
    Audio::GetInstance()->SoundPlayLoopEnd(bgmPlayHandle_);
}

void GameOver::SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size) {
    sprite.SetTexture(resourceManager->FindTexture(textureName));
    sprite.SetAnchor({ 0.5f,0.5f });
    sprite.SetTexcoordRect({ 0.0f,0.0f }, size);
    sprite.SetScale(size);
}

void GameOver::EasingClamp(float& t, float increaseValue) {
    if (t < 1.0f) {
        t += increaseValue;
    }
    if (t > 1.0f) {
        t = 1.0f;
    }
}