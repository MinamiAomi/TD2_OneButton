#pragma once

#include "Engine/Scene/BaseScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Input/Input.h"
#include "Engine/Graphics/Sprite.h"

#include "Graphics/ResourceManager.h"

class StageSerect : public BaseScene {

public:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private://メンバ関数
	//ステージ１を選んでいるときの処理
	void State1Update();

	//ステージ１を選んでいるときの処理
	void State2Update();

	//ステージ１を選んでいるときの処理
	void State3Update();

	/// <summary>
	/// シーンチェンジ
	/// </summary>
	void ChangeScene();

	/// <summary>
	/// スプライトの基本初期処理
	/// </summary>
	void SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size);

	/// <summary>
	/// イージングで使用するtの処理
	/// </summary>
	void EasingClamp(float& t, float increaseValue);

private://メンバ変数

	Input* input_;
	ResourceManager* resourceManager;

	//ステージ選択
	enum State {
		kStage1,
		kStage2,
		kStage3
	};

	//どのステージを選んでいるかの状態
	State state_ = kStage1;

	// 中心座標
	const Vector2 kCenter_ = { 270.0f, 360.0f };

#pragma region スプライト

	Sprite bobTex_;
	Sprite michaelTex_;
	Sprite maxTex_;

#pragma endregion

};