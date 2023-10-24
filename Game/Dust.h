#pragma once

#include "Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"
#include "Externals/ImGui/imgui.h"
class Dust
{
public:
	Dust();
	~Dust();

	void Initalize(const Vector2& position);
	void Update();
	//void Draw();

private:
	//モデルのインスタンス
	std::unique_ptr<ToonModelInstance> model_;
	//テクスチャの位置
	Transform world_;

	int AnimeFrame_ = 0;

	bool isAlive_ = true;
};

