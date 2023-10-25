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

#pragma region ゲッター
	
#pragma endregion

private:

	ToonModelInstance model_;
	Transform world_;


	Vector3 velo = { 0.0f,1.5f,0.0f };

	
	bool isActive_=false;

	int deadCount_;
};
