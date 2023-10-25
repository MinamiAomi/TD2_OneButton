#pragma once
#include "Engine/Scene/BaseScene.h"

#include <memory>

#include "Engine/Graphics/ToonModel.h"
#include "Math/Camera.h"
#include "Math/Transform.h"
#include"Engine/Input/Input.h"

#include "TitleLogo.h"
#include "TitlePlayer.h"
#include "TitleLaser.h"
#include "Background.h"

class TitleScene : public BaseScene {
public:
    void OnInitialize() override;
    void OnUpdate() override;
    void OnFinalize() override;

    /// <summary>
    /// シーン転換処理
    /// </summary>
    void ChangeScene();
private:
    Camera camera_;

    Input* input_;

    std::unique_ptr<TitleLogo> logo_;
    std::unique_ptr<TitlePlayer> player_;
    std::unique_ptr<TitleLaser> laser_;
    std::unique_ptr<Background> background_;
    std::unique_ptr<Sprite> pushSpace_;
    float pushSpaceAnimation_;
    size_t bgmPlayHandle_;
};