#pragma once
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"
#include<string>

class Boss {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">出現座標</param>
	/// <param name="toonModel">モデル情報</param>
	void Initalize(const Vector3& position);

	/// <summary>
	/// 値設定
	/// </summary>
	void ValueSetting();


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ボスに当たっているか
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="wide">半径</param>
	/// <returns>当たっているか否か</returns>
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
	/// <returns>worldTransformTranslate</returns>
	const Vector3 GetMatWT() {
		return {
			world_.worldMatrix.m[3][0],
			world_.worldMatrix.m[3][1],
			world_.worldMatrix.m[3][2]
		};
	}
#pragma endregion

#pragma region オンコリ
	//ボスのダメージくらう処理
	void OnCollisionExplosion(int dmg);

	//ボス回復処理
	void OnCollisionHealing();
#pragma endregion

	
private:
	//座標
	Transform world_;
	//モデル描画用
	ToonModelInstance modelInstance_;

	//ボスの上側の座標Y
	float bossYLine_;

	//中心点からの高さ
	float height_;

	
	//HP最大値
	int maxHP_ = 100;
	//ヒットポイント
	int HP_ = maxHP_;

	std::string groupName_ = "Boss";
};