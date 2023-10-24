#pragma once

#include "Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"
#include "Externals/ImGui/imgui.h"
class Heal {
public:

	Heal();
	~Heal();

	void Initalize(const Vector2& position);
	void Update();
	//void Draw();

	bool GetisAlive() { return isAlive_; }

private:
	//モデルのインスタンス
	std::unique_ptr<ToonModelInstance> model_;
	//テクスチャの位置
	Transform world_;

	int AnimeFrame_ = 0;

	bool isAlive_ = true;
};