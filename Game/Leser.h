#pragma once
#pragma once

//kamataEngine
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"

class Leser {
public:
	Leser();
	~Leser();

	void Initalize(std::shared_ptr<ToonModel> model, const Transform& PlayerworldTransform_);
	void Update();
	//void Draw();
	void OnCollision();
	bool GetIsAlive() { return IsAlive; }

private:


	Transform worldTransform_;
	//モデル描画
	ToonModelInstance modelInstance_;
	//
	int AliveCount = 240;
	bool IsAlive = true;
};
