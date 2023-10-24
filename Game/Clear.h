#pragma once

#include"Engine/Scene/BaseScene.h"
#include"Engine/Scene/SceneManager.h"
#include "Engine/Input/Input.h"
#include"Engine/Graphics/Sprite.h"
#include"Engine/Math/Transform.h"

#include"ClearLimit.h"

#include<memory>



class Clear : public BaseScene {
public:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

	/// <summary>
	/// シーンチェンジに関する処理
	/// </summary>
	void SceneChange();

private:
	//キー入力
	Input* input_;

	//クリアの文字
	Sprite clearTex_;
	//座標
	Transform clearTrans_;

	//残りスコア
	int limitScore_ = 0;
	//ボブかどうか（0=false,1=true)
	bool isBob_ = 0;

	//リミットクラス
	std::unique_ptr<ClearLimit>clearLimit_ = nullptr;
};