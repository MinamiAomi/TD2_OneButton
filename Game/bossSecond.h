#pragma once
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"
#include "Graphics/Sprite.h"

#include<string>

#include"BossSpike.h"
#include"dangerZone.h"
#include"shotSpike.h"

class BossSecond {
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

	/// <summary>
	/// ボスの攻撃に当たったどうか
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="wide"></param>
	bool IsHitBossATK(const Vector3& pos, const float& wide);

	//飛ばしたとげ都のコリジョン
	bool IsHitShotSpike(const Vector3& pos, const float wide);

	/// <summary>
	/// ボスの攻撃関連
	/// </summary>
	void BossATK();
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

	bool GetBossATKSpikeExplo() {
		if (atkType_ == kSpikeExpATK && atkWave_ == kWave1) {
			return true;
		}
		return false;
	}
#pragma endregion

#pragma region オンコリ
	//ボスのダメージくらう処理
	void OnCollisionExplosion(int dmg);

	//ボス回復処理
	void OnCollisionHealing(int dmg);
#pragma endregion


private:

	int GetRandomNum(int wideOrmax, bool isWide);
	//棘攻撃処理
	void SpikeAttack();

	//張り飛ばし
	void ShotSpikeATK();
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

	std::string groupName_ = "Boss2";

	bool isDead_ = false;


#pragma region 攻撃関連
	enum ATKType {
		kNone,//何もしない
		kSpikeExpATK, //攻撃
		kSpikeShot,//針飛ばし
	};

	//攻撃状態
	ATKType atkType_ = kNone;

	//攻撃のウェーブ管理
	enum ATKWave {
		kSetup,	//準備処理
		kWave1,
		kWave2,
		kWave3,
		Revert,
	};

	//攻撃の動きの段階処理
	ATKWave atkWave_ = kSetup;
	//ウェーブごとの初期化
	bool waveInitialize_ = false;

	//攻撃待機の待ち時間
	int WaitATKCount_ = 0;

	//与える待ち時間の
	const int maxWaitATKCount = 60 * 10;

	bool IsCount_ = true;

	//モーションに使うT
	float animetionT_ = 0;
#pragma endregion

#pragma region 棘飛ばして爆破攻撃

	std::unique_ptr<BossSpike>bossSpike_;

	//==SetUp
	const Vector2 setUpScale = { 1.0f,0.90f };

	//攻撃構える処理１のアニメーション加算地
	const float addSetUpAnimation_ = 1.0f / 60.0f;

	//==WAVE1==
	const Vector2 wave1Scale = { setUpScale.y,2.0f };

	//攻撃構える処理2のアニメーション加算地
	const float addWave1Animation_ = 1.0f / 45.0f;

	//==WAVE2==

	//攻撃構える処理3のアニメーション加算地
	const float addWave2Animation_ = 1.0f / 10.0f;

	//==WAVE3==
	const float addWave3Animation_ = 1.0f / 60.0f;

	//==Revert==
	const Vector2 revertScale = { wave1Scale.y,1.0f };
	const float addRevertAnimation_ = 1.0f / 30.0f;

#pragma endregion

#pragma region ボスの張り飛ばし攻撃
	std::unique_ptr<ShotSpike>shotSpike_;

	//発射位置
	Vector3 Spos[3] = {
		{-15.0f + 5,0,0,},
		{-15.0f + (5 * 3),0,0,},
		{-15.0f + (5 * 5),0,0,}
	};

	int randomNum_ = 0;

	Vector3 dZoneP[3] = {
		{Spos[0].x,-40,-5.0f},
		{Spos[1].x,-40,-5.0f},
		{Spos[2].x,-40,-5.0f}
	};

	Vector3 dzoneScale = { 6,18,1 };
	
#pragma endregion


	std::unique_ptr<DangerZone>dBossATKSpikeZone_;
	Vector3 dspikeZonePos = { 0.0f,-55.0f,-5.0f };
	Vector3 dspikeZoneScale = { 18.0f,10.0f,1.0f };


#pragma region HPバー
	std::unique_ptr<Sprite> HPBar_;
	Transform HPBarTrans_;
	std::unique_ptr<Sprite> HPBarFrame_;
	Transform HPBarFrameTrans_;

	const Vector2 rects_ = { 540,40 };
	Vector3 HPPos = { 270,20,0 };

	Vector2 scale = rects_;
#pragma endregion
};