#include"StageSerect.h"
#include"ImGame.h"

void StageSerect::OnInitialize() {
	//インスタンス取得
	input_ = Input::GetInstance();
}

void StageSerect::OnUpdate() {
}

void StageSerect::OnFinalize() {

	//１キーでゲーム
	if (input_->IsKeyTrigger(DIK_1)) {
		//インスタンス取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		//シーン設定
		sceneManager->ChangeScene<InGame>();

	}
}
