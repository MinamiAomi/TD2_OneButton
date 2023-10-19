#include "TitleScene.h"

#include "Engine/Graphics/ModelLoader.h"
#include "Engine/Graphics/RenderManager.h"
#include "Engine/Scene/SceneManager.h"
#include "Externals/ImGui/imgui.h"

void TitleScene::OnInitialize() {

    RenderManager::GetInstance()->SetCamera(camera_);

    titleLogo_ = std::make_unique<TitleLogo>();
    titleLogo_->Initialize();
    E_BossHeal = std::make_shared<Texture>();
    E_BossHeal->Load("Resources/Image/Heal.png");
    sprite.SetTexture(E_BossHeal);
    sprite.SetIsActive(true);
    sprite.SetDrawOrder(0);
    sprite.SetPosition(TexPos_.translate.GetXY());
    sprite.SetRotate(0.0f);
    sprite.SetScale({ 32.0f,32.0f });
    sprite.SetTexcoordRect({ 0.0f,0.0f }, { 32,32 });
}

void TitleScene::OnUpdate() {

    titleLogo_->Update();
#ifdef _DEBUG
    ImGui::Begin("Texture");
    ImGui::DragFloat2("Position", &TexPos_.translate.x, 0.1f);
    sprite.SetPosition(TexPos_.translate.GetXY());
    ImGui::End();
#endif

    camera_.UpdateMatrices();
}

void TitleScene::OnFinalize() {

}
