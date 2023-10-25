#include "TitleLaser.h"

#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"

void TitleLaser::Initialize() {
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    transform_.translate = {0.0f, -2.0f, 0.0f};
    startWidth_ = 1.5f;
    endWidth_ = 0.0f;
    transform_.scale = { startWidth_,10.0f,startWidth_ };
    transform_.UpdateMatrix();

    model_.SetModel(resourceManager->FindModel("Laser"));
    model_.SetPass(ToonModelInstance::Pass::Translucent);
    model_.SetAlpha(0.5f);

    explosionSprite_.SetTexture(resourceManager->FindTexture("Explosion"));
    explosionSprite_.SetDrawOrder(0);
    explosionSprite_.SetPosition({540.0f * 0.5f, 200.0f});
    explosionSprite_.SetTexcoordRect({}, { 512.0f, 512.0f });
    explosionSprite_.SetScale({ 0.0f,0.0f });

    animationParameter_ = 0.0f;
}

void TitleLaser::Update() {
    
    animationParameter_ += 1.0f / kAnimationDuration;
    transform_.rotate = Quaternion::MakeForYAxis(1.0f * Math::ToRadian) * transform_.rotate;
    transform_.scale.x = transform_.scale.z = Math::Lerp(animationParameter_, startWidth_, endWidth_);

    explosionSprite_.SetScale(Vector2(Math::Lerp(animationParameter_, 0.0f, 1500.0f)));
    
#ifdef _DEBUG
    ImGui::Begin("Title");
    if (ImGui::TreeNodeEx("Laser", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Position", &transform_.translate.x, 0.1f);
        ImGui::DragFloat3("Scale", &transform_.scale.x, 0.1f);
        ImGui::SliderFloat("AnimationParameter", &animationParameter_, 0.0f, kAnimationDuration);
        ImGui::TreePop();
    }
    ImGui::End();
#endif // _DEBUG


    transform_.UpdateMatrix();
    model_.SetWorldMatrix(transform_.worldMatrix);
}
