#pragma once
#pragma once

//kamataEngine
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"

#include<vector>

class Leser {
public:
	Leser();
	~Leser();

	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="ATK">レーザー（０）と爆発（１）のモデル配列</param>
	/// <param name="playerPos">プレイヤー座標（発生地点座標）</param>
	/// <param name="bossPos">ボス座標（攻撃設置点座標)</param>
	void Initialize(const Vector3& playerPos, const Vector3& bossPos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// コリジョン
	/// </summary>
	void OnCollision(const int& num);

	/// <summary>
	/// ボスの攻撃
	/// </summary>
	void OnCollisionBoss();


	/// <summary>
	/// すでにその棘が当たっているか
	/// </summary>
	bool IsAlreadyHit(const int& num);

#pragma region ゲッター
	//動いているか否か
	bool GetIsAlive() { return IsAlive_; }
	//レーザーの半径取得
	const float GetLeserWide() { return variableWide_; }
	//爆発半径取得
	const float GetExplotionRadius() { return explosionRadius_; }

	//爆発の中心点取得
	const Vector3 GetExplosionPos() {
		return explosionpos_.translate;
	}

	bool GetIsAlreadyATKBoss() { return isAlreadyATKBoss_; }
#pragma endregion

	
private:

	
	//生成されてからフレームのカウント
	int AliveCount_ = 240;
	//動いてるか否か
	bool IsAlive_ = true;

	//処理t
	float t_ = 0;

	//処理速度
	const float addT_ = 1.0f / 10.0f;

	//すでにコリジョン処理をしたものを保存
	std::vector<int>alreadyHitBallNumber_;

#pragma region レーザー関連
	//transform
	Transform leserTransform_;
	//モデル描画
	ToonModelInstance leserModelInstance_;

	//レーザーの最大半径
	const float leserWide_ = 1.5f;

	//レーザーの終わりの半径
	const float endLeserWide_ = 0.0f;

	//変数変化量
	float variableWide_ = leserWide_;

	
#pragma endregion

#pragma region 爆発処理関連
	//爆発中心点
	Transform explosionpos_;
	//モデル
	//ToonModelInstance expModelInstance_;
	//爆発最大半径
	const float maxExplosionRadius_ = 5.0f;
	//最小爆発半径
	const float minExplosionRadius_ = 0.0f;

	//爆発半径の変数
	float explosionRadius_ = minExplosionRadius_;

#pragma endregion

	bool isAlreadyATKBoss_ = false;
};
