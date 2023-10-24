#include"StageSerect.h"
#include"ImGame.h"
#include"InGame2.h"
void StageSerect::OnInitialize() {
	//インスタンス取得
	input_ = Input::GetInstance();
}

void StageSerect::OnUpdate() {

#pragma region 左右入力受け取り
	//左右同時押し対策
	control_ = 0;

	//左右入力
	if (input_->IsKeyTrigger(DIK_LEFT) || input_->IsKeyTrigger(DIK_A)) {
		control_ += -1;
	}
	else if (input_->IsKeyTrigger(DIK_RIGHT) || input_->IsKeyTrigger(DIK_D)) {
		control_ += 1;
	}
	
#pragma endregion

	//選んでる状態ごとの更新
	switch (state_) {
	case StageSerect::kStage1:
		State1Update();
		break;
	case StageSerect::kStage2:
		State2Update();
		break;
	case StageSerect::kStage3:
		State3Update();
		break;
	default:
		break;
	}

	ChangeScene();
}

void StageSerect::OnFinalize() {



}

void StageSerect::State1Update() {


	//他ステ選択
	//右
	if (control_ == 1) {
		state_ = kStage2;
	}
	//左
	else if (state_ == -1) {
		
	}


}

void StageSerect::State2Update() {

	//他ステ選択
	//右
	if (control_ == 1) {
		state_ = kStage3;
	}
	//左
	else if (state_ == -1) {
		state_ = kStage1;
	}
}

void StageSerect::State3Update() {
	//他ステ選択
	//右
	if (control_ == 1) {
		
	}
	//左
	else if (state_ == -1) {
		state_ = kStage2;
	}
}

void StageSerect::ChangeScene() {
	//１キーでゲーム
	if (input_->IsKeyTrigger(DIK_SPACE)) {
		//インスタンス取得
		SceneManager* sceneManager = SceneManager::GetInstance();

		//選択しているステージに移動
		switch (state_) {
		case StageSerect::kStage1:
			//シーン設定
			sceneManager->ChangeScene<InGame>();
			break;

		case StageSerect::kStage2:
			//シーン設定
			sceneManager->ChangeScene<InGame2>();
			break;

		case StageSerect::kStage3:
			//シーン設定
			sceneManager->ChangeScene<InGame>();
			break;

		default:
			break;
		}

	}
}
