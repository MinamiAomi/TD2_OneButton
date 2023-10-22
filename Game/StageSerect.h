#pragma once
#include"Engine/Scene/BaseScene.h"
#include"Engine/Scene/SceneManager.h"
#include"Engine/Input/Input.h"

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
private://メンバ変数

	//キー入力
	Input* input_;

	//ステージ選択
	enum State {
		kStage1,
		kStage2,
		kStage3
	};

	//どのステージを選んでいるかの状態
	State state_ = kStage1;

	//同時押しの対策
	int control_ = 0;

};