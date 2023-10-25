#pragma once

#include "Engine/Graphics/ToonModel.h"
#include "Math/Transform.h"


class Spike {
public:


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="world">ワールドT</param>
	/// <param name="model">モデル</param>
	/// <param name="State">状態</param>
	/// <param name="velo">移動ベクトル</param>
	void Initialize(int num, Transform world, const float* bossYLine,int coalescenceCount = 1, int State = kStay, Vector3 velo = { 0.0f,-1.0f,0.0f });


	void Update();



#pragma region ゲッター
	//transform取得
	Transform GetWorld() const { return world_; }

	//Twt取得
	const Vector3 GetmatWtranstate() const {
		return {
			world_.worldMatrix.m[3][0],
			world_.worldMatrix.m[3][1],
			world_.worldMatrix.m[3][2],
		};
	}

	//半径取得
	const float GetWide() { return wide_; }

	//死亡フラグ取得
	bool IsDead() const { return isDead_; }

	//棘の管理番号取得
	const int GetIdentificationNum() { return spikeNum_; }

	//当たり判定がONなのかチェック
	const bool GetIsCollisionOnBoss() { return collisionOnForBoss_; }

	//当たり判定がONなのかチェック
	const bool GetIsCollisonOnPlayer() { return collisionOnForPlayer_; }

	//当たり判定がONなのかチェック
	const bool GetIsCollisonOnSpike() { return collisionOnForSpike_; }


	//埋まり切ったかチェック
	const bool GetCompleteFillUp() { return CompleteFillUp_; }

	//ボスに当たるときの状態をチェックする
	const bool IsStateStay() {
		if (state_ == kStay) {
			return true;
		}
		else {
			return false;
		}
	}

	bool IsStateFillUp() {
		if (state_ == kFillUp) {
			return true;
		}
		return false;
	}

	const bool IsDamageProcessing() {
		if (isExplosion_ && !isApplicationDamage) {
			return true;
		}
		return false;
	}

	const int GetDamege() { return damage_; }

	//合体数取得
	int GetCoalescenceCount() { return coalescenceCount_; }
#pragma endregion

#pragma region セッター
	//死亡セット
	void SetDead() { isDead_ = true; collisionOnForBoss_ = false; collisionOnForPlayer_ = false; collisionOnForSpike_ = false; }
#pragma endregion


#pragma region オンコリ
	//プレイヤーと当たった時
	void OnCollisionPlayer();
	//ボスと当たった時
	void OnCollisionBoss();
	//プレイヤーのレーザー攻撃に当たった時
	void OnCollisionPlayerBeam();
	//プレイヤーの攻撃の爆破に当たった時
	void OnCollisionPlayerExplosion(Vector3 ExpPos);

	//棘同士当たった時
	void OnCollisionSpike();

	//プレイヤーのため攻撃に当たった時
	void OnCollisionPlayerStump();

	//壁との当たり判定
	void OnCollisionWall();

	//ボスに攻撃を与えたときの処理
	void OnCollisionExplotionBoss();


	//ボスの攻撃に利用されたときのコリジョン
	void OnCollisionBossATK(Vector3 velo);

	//ボスの攻撃で爆破する
	void OnCollisionBossATKExplosion();
#pragma endregion


	enum State {

		kStay,			//木についている
		kFalling,		//落ちる		
		kFillUp,		//埋まる
		kExplosion,		//BAKUHATU★		
		kFlyAway,		//爆風に吹き飛ばされる		
		kNone			//None
	};

private://各状態の初期化と更新処理

	//状態が変わったかチェック
	bool ckeckStateChange_ = false;

	//状態が変わった際に初期化をする処理をまとめた関数
	void CheckAllStateInitialize();

	//状態ごとの更新処理をまとめた関数
	void StateUpdate();

#pragma region 各種初期化関数
	//木についている状態の初期化
	void Stay_Initialize();

	//落ちる状態の初期化
	void Falling_Initialize();

	//埋まる状態の初期化
	void FillUp_Initiaize();

	//爆発の初期化
	void Explosion_Initialize();

	//横に飛ぶ処理の追加
	void FlyAway_Initialize();
#pragma endregion


#pragma region	各種更新関数
	//更新処理
	//木についている状態の更新
	void Stay_Update();

	//落ちる状態の初期化
	void Falling_Update();

	//埋まる状態の初期化
	void FillUp_Update();

	//爆発の初期化
	void Explosion_Update();

	//横に飛ぶ処理の追加
	void FlyAway_Update();
#pragma endregion





private:

	//管理番号
	int spikeNum_ = 0;

	//ワールド
	Transform world_;
	//モデル
	ToonModelInstance modelInstance_;

	//仮
	float wide_;

	//状態
	State state_;

	//移動量
	Vector3 velocity_;

	//死亡判定
	bool isDead_ = false;

	//減速量
	const float gensoku_ = 1.0f / 1000.0f;

	//棘にかかる重力
	float gravity = -0.07f;


	//爆破アニメーションのカウント
	int animationCount_ = 0;

	//アニメーションの終わりカウント
	const int maxAnimationCount = 30;



	//ボスとコリジョン処理するか否か
	bool collisionOnForBoss_ = true;

	//プレイヤーのビームとコリジョン処理するか
	bool collisionOnForPlayer_ = true;

	//プパイクとの処理するか
	bool collisionOnForSpike_ = true;


	//無敵時間残りカウント
	int noCollisionCount_ = 0;


#pragma region 埋まる処理
	//ボスに埋まり切るかのカウント
	int fillUpCount_;
	//埋まるまでのマックス
	const int maxFillUpCount_ = 60*3;
	//埋まり切ったかのフラグ
	bool CompleteFillUp_ = false;

	//ボスのコリジョンの上部分
	const float* BossYLine_;
#pragma endregion


#pragma region 吹き飛ぶ処理
	//飛ぶ向きが左か否か
	bool veloLeft_;

	//爆風の時の初期速度
	Vector3 exploVec_ = { 0.5f,0.0f,0.0f };

	//吹っ飛ぶときの減算量
	const float addVeloX_ = 1.0f / 60.0f;

	//飛ぶ時間
	float flyAwayCount_ = 0;
	//飛ぶ瞬間の最大カウント
	const float maxFlyAwayCount_ = 30;

#pragma endregion
	//爆発しているか
	bool isExplosion_ = false;

	//ダメージ処理をしたか
	bool isApplicationDamage = false;

	//ダメージ量
	int damage_ = 100;

	//ボスの棘攻撃に当たったか
	bool IsCollisionBossSpikeATK_ = false;

	//爆発モデル
	ToonModelInstance exploModel_;
	//爆発座標
	Transform exploTrans_;

	//合体数
	int coalescenceCount_ = 1;
};
