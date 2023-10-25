#include "GlobalVariables.h"
#include "TitleScene.h"
#include "StageSerect.h"
#include "ImGame.h"
#include "InGame2.h"

void StageSerect::OnInitialize() {
	input_ = Input::GetInstance();
	resourceManager = ResourceManager::GetInstance();

#pragma region スプライト初期化

	SpriteInitialize(glayTex_, "whiteBackGround", { 540.0f, 720.0f });
	glayTex_.SetPosition({ kCenter_.x, kCenter_.y });
	glayTex_.SetDrawOrder(uint8_t(1));

	SpriteInitialize(bobTex_, "selectBob", { 540.0f, 720.0f });
	SpriteInitialize(michaelTex_, "selectMichael", { 540.0f, 720.0f });
	SpriteInitialize(maxTex_, "selectMax", { 540.0f, 720.0f });
	bobTex_.SetPosition({ kCenter_.x, kCenter_.y });
	michaelTex_.SetPosition({ kCenter_.x, kCenter_.y });
	maxTex_.SetPosition({ kCenter_.x, kCenter_.y });
	bobTex_.SetIsActive(false);
	michaelTex_.SetIsActive(false);
	maxTex_.SetIsActive(false);

	SpriteInitialize(bobPosterTex_, "bobPoster", { 474.0f, 303.0f });
	SpriteInitialize(michaelPosterTex_, "michaelPoster", { 474.0f, 303.0f });
	SpriteInitialize(maxPosterTex_, "maxPoster", { 474.0f, 303.0f });
	bobPosterTex_.SetPosition({ kCenter_.x, kCenter_.y + 75.0f });
	michaelPosterTex_.SetPosition({ kCenter_.x, kCenter_.y + 75.0f });
	maxPosterTex_.SetPosition({ kCenter_.x, kCenter_.y + 75.0f });
	bobPosterTex_.SetIsActive(false);
	michaelPosterTex_.SetIsActive(false);
	maxPosterTex_.SetIsActive(false);

	SpriteInitialize(pushSpaceToStartTex_, "pushSpaceToStart", { 460.0f, 50.0f });
	pushSpaceToStartTex_.SetPosition({ kCenter_.x, 100.0f });
	SpriteInitialize(toBeContinuedTex_, "toBeContinued", { 460, 50.0f });
	toBeContinuedTex_.SetPosition({ kCenter_.x, 100.0f });
	toBeContinuedTex_.SetIsActive(false);
#pragma endregion
}

void StageSerect::OnUpdate() {

	auto easeOutCirc = [](float t) { return std::sqrtf(1 - std::powf(t - 1, 2)); };
	float tmpColor = 80.0f / 255.0f;
	if (!isPushSpace_) {
		EasingClamp(glayFadeOutEasingT_, 0.02f);
		glayTex_.SetColor({ tmpColor, tmpColor, tmpColor, Math::Lerp(easeOutCirc(glayFadeOutEasingT_), 1.0f, 0.0f) });
		if (glayFadeOutEasingT_ == 1.0f && input_->IsKeyTrigger(DIK_SPACE) && state_ != kStage3) {
			glayFadeOutEasingT_ = 0.0f;
			isPushSpace_ = true;
		}
	}
	else {
		EasingClamp(glayFadeOutEasingT_, 0.02f);
		glayTex_.SetColor({ tmpColor, tmpColor, tmpColor, Math::Lerp(easeOutCirc(glayFadeOutEasingT_), 0.0f, 1.0f) });
	}


	EasingClamp(posterScaleEasingT_, 0.07f);
	float scaleMag = 1.2f;

	pushSpaceToStartTex_.SetScale(Vector2::Lerp(easeOutCirc(pushSpaceScaleEasingT_), { 460.0f * 1.2f, 50.0f * 1.2f }, { 460.0f, 50.0f }));
	EasingClamp(pushSpaceScaleEasingT_, 0.04f);
	if (pushSpaceScaleEasingT_ == 1.0f) {
		pushSpaceScaleEasingT_ = 0.0f;
	}

	// 選んでる状態ごとの更新
	switch (state_) {
	case StageSerect::kStage1:
		State1Update();
		bobTex_.SetIsActive(true);
		bobPosterTex_.SetIsActive(true);
		bobPosterTex_.SetScale(Vector2::Lerp(easeOutCirc(posterScaleEasingT_), { 474.0f * scaleMag, 303.0f * scaleMag }, { 474.0f, 303.0f }));

		michaelTex_.SetIsActive(false);
		michaelPosterTex_.SetIsActive(false);
		break;
	case StageSerect::kStage2:
		State2Update();
		bobTex_.SetIsActive(false);
		bobPosterTex_.SetIsActive(false);

		michaelTex_.SetIsActive(true);
		michaelPosterTex_.SetIsActive(true);
		michaelPosterTex_.SetScale(Vector2::Lerp(easeOutCirc(posterScaleEasingT_), { 474.0f * scaleMag, 303.0f * scaleMag }, { 474.0f, 303.0f }));

		maxTex_.SetIsActive(false);
		maxPosterTex_.SetIsActive(false);

		pushSpaceToStartTex_.SetIsActive(true);
		toBeContinuedTex_.SetIsActive(false);
		break;
	case StageSerect::kStage3:
		State3Update();
		michaelTex_.SetIsActive(false);
		michaelPosterTex_.SetIsActive(false);

		maxTex_.SetIsActive(true);
		maxPosterTex_.SetIsActive(true);
		maxPosterTex_.SetScale(Vector2::Lerp(easeOutCirc(posterScaleEasingT_), { 474.0f * scaleMag, 303.0f * scaleMag }, { 474.0f, 303.0f }));

		pushSpaceToStartTex_.SetIsActive(false);
		toBeContinuedTex_.SetIsActive(true);
		break;
	}

	ChangeScene();
}

void StageSerect::OnFinalize() {
}

void StageSerect::State1Update() {
	// 右
	if (input_->IsKeyTrigger(DIK_RIGHT) || input_->IsKeyTrigger(DIK_D)) {
		posterScaleEasingT_ = 0.0f;
		state_ = kStage2;
	}
}

void StageSerect::State2Update() {
	// 右
	if (input_->IsKeyTrigger(DIK_RIGHT) || input_->IsKeyTrigger(DIK_D)) {
		posterScaleEasingT_ = 0.0f;
		state_ = kStage3;
	}
	// 左
	else if (input_->IsKeyTrigger(DIK_LEFT) || input_->IsKeyTrigger(DIK_A)) {
		posterScaleEasingT_ = 0.0f;
		state_ = kStage1;
	}
}

void StageSerect::State3Update() {
	// 左
	if (input_->IsKeyTrigger(DIK_LEFT) || input_->IsKeyTrigger(DIK_A)) {
		posterScaleEasingT_ = 0.0f;
		state_ = kStage2;
	}
}

void StageSerect::ChangeScene() {
	// ESCキーでタイトル
	if (input_->IsKeyTrigger(DIK_ESCAPE)) {
		SceneManager* sceneManager = SceneManager::GetInstance();
		sceneManager->ChangeScene<TitleScene>();
	}
	// スペースキーでゲーム
	if (glayFadeOutEasingT_ == 1.0f && isPushSpace_) {
		// インスタンス取得
		SceneManager* sceneManager = SceneManager::GetInstance();

		// 選択しているステージに移動
		switch (state_) {
		case StageSerect::kStage1:
			// シーン設定
			sceneManager->ChangeScene<InGame>();
			break;

		case StageSerect::kStage2:
			// シーン設定
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