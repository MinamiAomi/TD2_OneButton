#pragma once

#include "Engine/Graphics/ToonModel.h"
#include "Math/Transform.h"

class BossSpike {
public:
	//初期化
	void Initialize(Transform world);

	//初期状態に戻す
	void SetStart();

	//棘を伸ばす更新
	void UpdateExtendSpike(const float& t);

	//棘を小さくする更新
	void UpdateShrinkSpike(const float& t);

#pragma region ゲッター
	float GetATKScaleNum() { return world_.scale.y; }
#pragma endregion

private:

	ToonModelInstance model_;
	Transform world_;


	Vector2 extendScale = { 0.0f,6.0f };

	Vector2 shrinkScale = { extendScale.y,0.0f };
};
