#include"Clear.h"
#include"TitleScene.h"
#include"ImGame.h"

void Clear::OnInitialize() {
	sceneManager_ = SceneManager::GetInstance();
	input_ = Input::GetInstance();
}

void Clear::OnUpdate() {

	
	//シーンチェンジ処理
	SceneChange();
}

void Clear::OnFinalize() {
}

void Clear::SceneChange() {
	//スぺースキーでタイトル
	if (input_->IsKeyTrigger(DIK_1)) {
		//シーン設定
		sceneManager_->ChangeScene<InGame>();

	}
}
