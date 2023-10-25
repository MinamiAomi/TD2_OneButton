#include "Externals/ImGui/imgui.h"
#include "GlobalVariables.h"
#include "Clear.h"
#include "StageSerect.h"
#include "ImGame.h"
#include "Audio/Audio.h"

void Clear::OnInitialize() {
    input_ = Input::GetInstance();

#pragma region 画像初期化

    resourceManager = ResourceManager::GetInstance();
    // 背景
    SpriteInitialize(backGroundTex_, "clearBackGround", { 540.0f, 720.0f });
    backGroundTex_.SetPosition({ center.x, center.y });
    // YouWin
    SpriteInitialize(youWinTex_, "youWin", { 400.0f, 64.0f });
    youWinTex_.SetPosition({ center.x, center.y });
    // GoodBye
    SpriteInitialize(goodByeTex_, "goodBye", { 300.0f, 48.0f });
    goodByeTex_.SetIsActive(false);
    // Bob
    SpriteInitialize(bobTex_, "bob", { 200.0f, 80.0f });
    bobTex_.SetPosition({ 400.0f, 600.0f });
    bobTex_.SetIsActive(false);
    SpriteInitialize(bobBodyTex_, "bobBody", { 700.0f, 300.0f });
    bobBodyTex_.SetIsActive(false);
    // Michael
    SpriteInitialize(michaelTex_, "michael", { 400.0f, 80.0f });
    michaelTex_.SetPosition({ 400.0f, 600.0f });
    michaelTex_.SetIsActive(false);
    SpriteInitialize(michaelBodyTex_, "michaelBody", { 700.0f, 300.0f });
    michaelBodyTex_.SetIsActive(false);
    // 背景
    SpriteInitialize(blackBackGroundTex_, "whiteBackGround", { 540.0f, 720.0f });
    blackBackGroundTex_.SetPosition({ center.x, center.y });
    blackBackGroundTex_.SetScale({ 0.0f, 720.0f });
    blackBackGroundTex_.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    blackBackGroundTex_.SetDrawOrder(uint8_t(1));
    SpriteInitialize(glayBackGroundTex_, "whiteBackGround", { 540.0f, 720.0f });
    glayBackGroundTex_.SetPosition({ center.x, center.y });
    glayBackGroundTex_.SetScale({ 0.0f, 720.0f });
    float colorTmp = 80.0f / 255.0f;
    glayBackGroundTex_.SetColor({ colorTmp, colorTmp, colorTmp, 1.0f });
    glayBackGroundTex_.SetDrawOrder(uint8_t(2));


#pragma endregion

#pragma region スコア取得

    const char dataName[] = "data";

    // インスタンス取得
    GlobalVariables* globalV = GlobalVariables::GetInstance();
    // グループの追加
    GlobalVariables::GetInstance()->CreateGroup(dataName);

    std::string keyLimit = "Limit";
    std::string IsBob = "IsBob";

    // 各パラメータの取得
    limitScore_ = globalV->GetIntvalue(dataName, keyLimit);
    isBob_ = globalV->GetIntvalue(dataName, IsBob);

#pragma endregion

    clearLimit_ = std::make_unique<ClearLimit>();
    clearLimit_->Initialize(limitScore_, isBob_);


    Audio* audio = Audio::GetInstance();
    bgmPlayHandle_ = audio->SoundPlayLoopStart(resourceManager->FindSound("ResultBGM"));
    audio->SetValume(bgmPlayHandle_, 0.2f);
}

void Clear::OnUpdate() {

    frame_++;

    if (frame_ > 180) {
        clearLimit_->Update(frame_);
    }
    SpriteAnimation();

    // シーンチェンジ処理
    SceneChange();
}

void Clear::OnFinalize() {
    Audio::GetInstance()->SoundPlayLoopEnd(bgmPlayHandle_);
}

void Clear::SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size) {
    sprite.SetTexture(resourceManager->FindTexture(textureName));
    sprite.SetAnchor({ 0.5f,0.5f });
    sprite.SetTexcoordRect({ 0.0f,0.0f }, size);
    sprite.SetScale(size);
}

void Clear::SceneChange() {
    // SPACEキーでゲームシーン
    if (frame_ > 420 && input_->IsKeyTrigger(DIK_SPACE)) {
        isPushSpace_ = true;
    }
    if (isPushSpace_) {
        auto easeOutCirc = [](float t) { return std::sqrtf(1 - std::powf(t - 1, 2)); };
        EasingClamp(blackScaleEasingT_, 0.02f);
        blackBackGroundTex_.SetScale({ Math::Lerp(easeOutCirc(blackScaleEasingT_), 0.0f, 540.0f), 720.0f });
        if (blackScaleEasingT_ > 0.5f) {
            EasingClamp(glayScaleEasingT_, 0.02f);
            glayBackGroundTex_.SetScale({ Math::Lerp(easeOutCirc(glayScaleEasingT_), 0.0f, 540.0f), 720.0f });
            if (glayScaleEasingT_ == 1.0f) {
                // インスタンス取得
                SceneManager* sceneManager = SceneManager::GetInstance();
                // シーン設定
                sceneManager->ChangeScene<StageSerect>();
            }
        }
    }
}

void Clear::SpriteAnimation() {

    auto easeOutCirc = [](float t) { return std::sqrtf(1 - std::powf(t - 1, 2)); };
    auto easeInSine = [](float t) { return 1 - std::cosf((t * Math::Pi) / 2); };

    // YouWin
    if (!isCompleteYouWinFadeIn_) {
        EasingClamp(youWinFadeInEasingT_, 0.02f);
        if (youWinFadeInEasingT_ == 1.0f) {
            isCompleteYouWinFadeIn_ = true;
        }
        youWinTex_.SetColor({ 1.0f, 1.0f, 1.0f, easeOutCirc(youWinFadeInEasingT_) });
    }
    else if (frame_ > 40)
    {
        EasingClamp(youWinFadeOutEasingT_, 0.02f);
        youWinFadeInEasingT_ = 1.0f - youWinFadeOutEasingT_;
        youWinTex_.SetColor({ 1.0f, 1.0f, 1.0f, easeInSine(youWinFadeInEasingT_) });
    }
    // GoodBye
    if (frame_ > 90) {
        EasingClamp(goodByeEasingT_, 0.02f);
        goodByePosition_ = Vector2::Lerp(easeOutCirc(goodByeEasingT_), { -200.0f, 675.0f }, { 150.0f, 675.0f });
        goodByeTex_.SetPosition(goodByePosition_);
        goodByeTex_.SetIsActive(true);
    }
    if (frame_ > 120) {
        if (nameScaleEasingT_ == 1.0f) {
            nameScaleEasingT_ = 0.0f;
        }
        EasingClamp(nameScaleEasingT_, 0.04f);
        EasingClamp(bossBodyFadeInEasingT_, 0.02f);
        bossBodySinMoveT_ += 0.05f;
        // Bob
        if (isBob_) {
            bobTex_.SetScale(Vector2::Lerp(1.0f - easeInSine(nameScaleEasingT_), { 200.0f, 80.0f }, { 250.0f, 100.0f }));
            bobTex_.SetIsActive(true);
            bobBodyTex_.SetColor({ 1.0f, 1.0f, 1.0f, Math::Lerp(easeOutCirc(bossBodyFadeInEasingT_), 0.0f, 1.0f) });
            bobBodyTex_.SetPosition({ 270.0f, 40.0f + 8.0f * std::sinf(bossBodySinMoveT_) });
            bobBodyTex_.SetIsActive(true);
        }
        // Michael
        else {
            michaelTex_.SetScale(Vector2::Lerp(1.0f - easeInSine(nameScaleEasingT_), { 250.0f, 50.0f }, { 312.5f, 62.5f }));
            michaelTex_.SetIsActive(true);
            michaelBodyTex_.SetColor({ 1.0f, 1.0f, 1.0f, Math::Lerp(easeOutCirc(bossBodyFadeInEasingT_), 0.0f, 1.0f) });
            michaelBodyTex_.SetPosition({ 270.0f, 40.0f + 8.0f * std::sinf(bossBodySinMoveT_) });
            michaelBodyTex_.SetIsActive(true);
        }
    }
}

void Clear::EasingClamp(float& t, float increaseValue) {
    if (t < 1.0f) {
        t += increaseValue;
    }
    if (t > 1.0f) {
        t = 1.0f;
    }
}
