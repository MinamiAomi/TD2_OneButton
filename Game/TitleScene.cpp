#include "TitleScene.h"

#include "Engine/Graphics/ModelLoader.h"
#include "Engine/Graphics/RenderManager.h"
#include "Engine/Scene/SceneManager.h"

void TitleScene::OnInitialize() {

    RenderManager::GetInstance()->SetCamera(camera_);


}

void TitleScene::OnUpdate() {
    camera_.UpdateMatrices();

}

void TitleScene::OnFinalize() {

}
