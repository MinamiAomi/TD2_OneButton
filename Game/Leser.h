#pragma once
#pragma once

//kamataEngine
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"

class Leser {
public:
	Leser();
	~Leser();

	//初期化
	void Initalize(std::shared_ptr<ToonModel> model, const Vector3& playerPos, const Vector3& bossPos);
	
	void Update();
	
	void OnCollision();

#pragma region ゲッター
	bool GetIsAlive() { return IsAlive; }

	const float GetWide() { return variableWide_; }
#pragma endregion

	
private:


	Transform worldTransform_;
	//モデル描画
	ToonModelInstance modelInstance_;
	//
	int AliveCount = 240;
	bool IsAlive = true;


	//プレイヤー座標
	const Transform* playerT_;

	//レーザーの半径
	const float leserWide_ = 0.5f;

	//変数変化量
	float variableWide_ = leserWide_;

	//１フレームに小さくするスケールの量
	const float SubWide_ = (1.0f * leserWide_) / (30.0f * leserWide_);


};
