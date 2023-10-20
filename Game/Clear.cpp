#include"Clear.h"
#include"TitleScene.h"
#include"ImGame.h"

void Clear::OnInitialize() {
	input_ = Input::GetInstance();
}

void Clear::OnUpdate() {


	//シーンチェンジ処理
	SceneChange();
}

void Clear::OnFinalize() {
}

void Clear::SceneChange() {
	//1キーでゲームシーン
	if (input_->IsKeyTrigger(DIK_1)) {
		//インスタンス取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		//シーン設定
		sceneManager->ChangeScene<InGame>();
	}
}
