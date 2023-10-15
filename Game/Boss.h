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



	void OnCollisionExplosion(int dmg);

private:
	//座標
	Transform world_;
	//モデル描画用
	ToonModelInstance modelInstance_;

};