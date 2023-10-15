#pragma once
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"


class Boss {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">出現座標</param>
	/// <param name="toonModel">モデル情報</param>
	void Initalize(const Vector3& position, std::shared_ptr<ToonModel> toonModel);

	void Update();


#pragma region ゲッター
	float GetBossY() { return world_.worldMatrix.m[3][1]; }
#pragma endregion

#pragma region オンコリ
	void OnCollisionExplosion(int dmg);
#pragma endregion

	
private:
	//座標
	Transform world_;
	//モデル描画用
	ToonModelInstance modelInstance_;

	float wide_;
};