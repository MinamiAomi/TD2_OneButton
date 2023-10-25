#pragma once

#include "Engine/Graphics/ToonModel.h"
#include "Math/Transform.h"

class ShotSpike {
public:
	//初期化
	void Initialize();

	//打つ
	void Shot(Vector3 pos);
	
	void Update();

	bool Collision(const Vector3& pos, const float wide);
#pragma region ゲッター
	
#pragma endregion

private:

	ToonModelInstance model_;
	Transform world_;


	Vector3 velo = { 0.0f,1.5f,0.0f };

	
	bool isActive_=false;

	int deadCount_;

	Vector2 size_ = { 10.0f,2.2f };
};
