#pragma once

#include"Engine/Scene/BaseScene.h"
#include"Engine/Scene/SceneManager.h"
#include "Engine/Input/Input.h"


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

	// 
	
};