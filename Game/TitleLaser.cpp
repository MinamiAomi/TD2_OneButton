#include "TitleLaser.h"

#include "Graphics/ResourceManager.h"

void TitleLaser::Initialize() {
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    transform_.translate = {0.0f, -5.0f, 0.0f};
    transform_.UpdateMatrix();

    model_.SetModel(resourceManager->FindModel("Laser"));
    model_.SetPass(ToonModelInstance::Pass::Translucent);
    model_.SetAlpha(0.5f);

    animationParameter_ = 0;
}

void TitleLaser::Update() {
    ++animationParameter_;

    model_.SetWorldMatrix(transform_.worldMatrix);
}
