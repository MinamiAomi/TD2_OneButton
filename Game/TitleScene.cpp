#include "TitleScene.h"

#include "Engine/Graphics/ModelLoader.h"
#include "Engine/Graphics/RenderManager.h"
#include "Engine/Scene/SceneManager.h"
#include "Externals/ImGui/imgui.h"

void TitleScene::OnInitialize() {

    RenderManager::GetInstance()->SetCamera(camera_);

    titleLogo_ = std::make_unique<TitleLogo>();
    titleLogo_->Initialize();

}

void TitleScene::OnUpdate() {

    titleLogo_->Update();

    camera_.UpdateMatrices();
}

void TitleScene::OnFinalize() {

}
