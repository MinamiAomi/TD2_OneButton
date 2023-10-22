#include "TitleScene.h"

#include "Engine/Graphics/ModelLoader.h"
#include "Engine/Graphics/RenderManager.h"
#include "Engine/Scene/SceneManager.h"
#include "Externals/ImGui/imgui.h"

#include"StageSerect.h"

void TitleScene::OnInitialize() {

    RenderManager::GetInstance()->SetCamera(camera_);

	input_ = Input::GetInstance();

    titleLogo_ = std::make_unique<TitleLogo>();
    titleLogo_->Initialize();

}

void TitleScene::OnUpdate() {

    titleLogo_->Update();

    camera_.UpdateMatrices();

	ChangeScene();
}

void TitleScene::OnFinalize() {

}

void TitleScene::ChangeScene() {
	//SPACEキーでゲームシーン
	if (input_->IsKeyTrigger(DIK_SPACE)) {
		//インスタンス取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		//シーン設定
		sceneManager->ChangeScene<StageSerect>();
	}
}
