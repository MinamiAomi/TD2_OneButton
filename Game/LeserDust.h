#pragma once

#include "Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"
#include "Externals/ImGui/imgui.h"
class LeserDust
{
public:
	LeserDust();
	~LeserDust();

	void Initalize(const Vector2& position);
	void Update();
	//void Draw();

	bool GetisAlive() { return isAlive_; }

private:
	//モデルのインスタンス
	const int kMaxmodel = 5;
	std::unique_ptr<ToonModelInstance> model_[5];
	//テクスチャの位置
	Transform model_world[5];
	Vector2 model_move[5] = {
		{-0.15f,0.0f},
		{-0.1f,0.1f},
		{0.0f,0.15f},
		{0.1f,0.1f},
		{0.15f,0.0f},
	};

	int AnimeFrame_ = 0;

	bool isAlive_ = true;
};

