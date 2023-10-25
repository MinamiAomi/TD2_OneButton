#pragma once
#include"Math/Transform.h"
#include"Engine/Graphics/ToonModel.h"

class DangerZone {
public:

	//初期化処理
	void Initialize(Vector3 pos,Vector3 scale);

	void SetCount(int second, int loopNum);

	void Update();
private:
	//transform
	Transform world;
	//モデル描画
	ToonModelInstance model;

	//行動中かどうか
	bool isActive_ = false;

	float Alpha = 0.0f;

	const float alphaMax = 0.5f;
	const float alphaMin = 0.0f;

	float loopGetter;
	float loopCounter;


	float countOneF;
	float t_ = 0;

	bool isAdd_ = true;
	void LoopAnimation();
};