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

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ボスに当たっているか
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="wide">半径</param>
	/// <returns></returns>
	bool IsHitBoss(const Vector3& pos, const float& wide);

#pragma region ゲッター
	/// <summary>
	/// ボスの上のラインを取得
	/// </summary>
	/// <returns>Y座標</returns>
	const float& GetBossYLine() { return bossYLine_; }

	/// <summary>
	/// ボスのmatT取得
	/// </summary>
	/// <returns>wtt</returns>
	const Vector3 GetmatWT() {
		return {
			world_.worldMatrix.m[3][0],
			world_.worldMatrix.m[3][1],
			world_.worldMatrix.m[3][2]
		};
	}
#pragma endregion

#pragma region オンコリ
	void OnCollisionExplosion(int dmg);

	//ボス回復処理
	void OnCollisionHealing();
#pragma endregion

	
private:
	//座標
	Transform world_;
	//モデル描画用
	ToonModelInstance modelInstance_;

	float bossYLine_;

	float wide_;

	//ヒットポイント
	int HP_;

	//最大値
	const int maxHP_ = 100;
};