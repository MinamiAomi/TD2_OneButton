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

	const Vector3 GetExplosionPos() {
		return explosionpos_.translate;
	}
#pragma endregion

	
private:

	
	//
	int AliveCount = 240;
	//動いてるか否か
	bool IsAlive = true;

	//処理t
	float t_ = 0;

	//処理速度
	const float addT_ = 1.0f / 30.0f;


#pragma region レーザー関連
	//transform
	Transform worldTransform_;
	//モデル描画
	ToonModelInstance modelInstance_;

	//レーザーの半径
	const float leserWide_ = 0.5f;

	//レーザーの終わりの半径
	const float endLeserWide_ = 0.0f;

	//変数変化量
	float variableWide_ = leserWide_;

	
#pragma endregion

#pragma region 爆発処理関連
	//モデル
	ToonModelInstance expModelInstance_;
	//爆発中心点
	Transform explosionpos_;
	//爆発最大点
	const float maxExplosionRadius_ = 5.0f;

	const float minExplosionRadius_ = 0.0f;


	//爆発半径の変数
	float explosionRadius_ = 0.0f;

#pragma endregion

	
};
