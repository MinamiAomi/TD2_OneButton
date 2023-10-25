#include "TitleScene.h"

#include "Engine/Graphics/ModelLoader.h"
#include "Engine/Graphics/RenderManager.h"
#include "Engine/Scene/SceneManager.h"
#include "Externals/ImGui/imgui.h"

#include"StageSerect.h"

void TitleScene::OnInitialize() {

    RenderManager::GetInstance()->SetCamera(camera_);
	camera_.SetPosition({ 0.0f, 0.0f, -100.0f });
	camera_.SetRotate({});
	camera_.SetPerspective(25.0f * Math::ToRadian, 540.0f / 720.0f, 50.0f, 200.0f);
	camera_.UpdateMatrices();

	input_ = Input::GetInstance();

    logo_ = std::make_unique<TitleLogo>();
    logo_->Initialize();

	player_ = std::make_unique<TitlePlayer>();
	player_->Initialize();
}

void TitleScene::OnUpdate() {

    logo_->Update();
	player_->Update();

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
