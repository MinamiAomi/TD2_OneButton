#include "GlobalVariables.h"
#include "StageSerect.h"
#include "ImGame.h"
#include "InGame2.h"

void StageSerect::OnInitialize() {
	input_ = Input::GetInstance();
	resourceManager = ResourceManager::GetInstance();

#pragma region スプライト初期化

	SpriteInitialize(bobTex_, "selectBob", { 540.0f, 720.0f });
	bobTex_.SetPosition({ kCenter_.x, kCenter_.y });
	bobTex_.SetIsActive(false);
	SpriteInitialize(michaelTex_, "selectMichael", { 540.0f, 720.0f });
	michaelTex_.SetPosition({ kCenter_.x, kCenter_.y });
	michaelTex_.SetIsActive(false);
	SpriteInitialize(maxTex_, "selectMax", { 540.0f, 720.0f });
	maxTex_.SetPosition({ kCenter_.x, kCenter_.y });
	maxTex_.SetIsActive(false);

#pragma endregion
}

void StageSerect::OnUpdate() {

	//選んでる状態ごとの更新
	switch (state_) {
	case StageSerect::kStage1:
		State1Update();
		michaelTex_.SetIsActive(false);
		bobTex_.SetIsActive(true);
		break;
	case StageSerect::kStage2:
		State2Update();
		bobTex_.SetIsActive(false);
		michaelTex_.SetIsActive(true);
		maxTex_.SetIsActive(false);
		break;
	case StageSerect::kStage3:
		State3Update();
		michaelTex_.SetIsActive(false);
		maxTex_.SetIsActive(true);
		break;
	}

	ChangeScene();
}

void StageSerect::OnFinalize() {
}

void StageSerect::State1Update() {
	//右
	if (input_->IsKeyTrigger(DIK_RIGHT) || input_->IsKeyTrigger(DIK_D)) {
		state_ = kStage2;
	}
}

void StageSerect::State2Update() {
	//右
	if (input_->IsKeyTrigger(DIK_RIGHT) || input_->IsKeyTrigger(DIK_D)) {
		state_ = kStage3;
	}
	//左
	else if (input_->IsKeyTrigger(DIK_LEFT) || input_->IsKeyTrigger(DIK_A)) {
		state_ = kStage1;
	}
}

void StageSerect::State3Update() {
	//左
	if (input_->IsKeyTrigger(DIK_LEFT) || input_->IsKeyTrigger(DIK_A)) {
		state_ = kStage2;
	}
}

void StageSerect::ChangeScene() {
	//スペースキーでゲーム
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
		}

	}
}

void StageSerect::SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size) {
	sprite.SetTexture(resourceManager->FindTexture(textureName));
	sprite.SetAnchor({ 0.5f,0.5f });
	sprite.SetTexcoordRect({ 0.0f,0.0f }, size);
	sprite.SetScale(size);
}

void StageSerect::EasingClamp(float& t, float increaseValue) {
	if (t < 1.0f) {
		t += increaseValue;
	}
	if (t > 1.0f) {
		t = 1.0f;
	}
}