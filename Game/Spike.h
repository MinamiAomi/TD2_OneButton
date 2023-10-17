#pragma once

#include "Engine/Graphics/ToonModel.h"
#include "Math/Transform.h"


class Spike {
public:
	enum SpikeState {
		// 木についている
		kStay,
		// 落ちる
		kFalling,
		// 埋まる
		kFillUp,
		// BAKUHATU★
		kExplosion,
		//爆風に吹き飛ばされる
		kFlyAway,
		//None
		kNone
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="world">ワールドT</param>
	/// <param name="model">モデル</param>
	/// <param name="State">状態</param>
	/// <param name="velo">移動ベクトル</param>
	void Initialize(int num,Transform world, std::shared_ptr<ToonModel> toonModel, int State=kStay, Vector3 velo = { 0.0f,-1.0f,0.0f });


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

	//加速度取得
	Vector3 GetVelocity_() const { return velocity_; }

	//死亡フラグ取得
	bool IsDead() const { return isDead_; }

	//棘の管理番号取得
	const int GetIdentificationNum() { return spikeNum_; }

	const bool GetIsCollisionOn() { return collision_on; }

	const bool GetCompleteFillUp() { return CompleteFillUp_; }

	const bool IsStateFillUp() {
		if (state_ == kFillUp) {
			return true;
		}
		else {
			return false;
		}
	}
#pragma endregion

#pragma region セッター
	//死亡セット
	void SetDead() { isDead_ = true; collision_on = false; }
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

	void OnCollisionWall();
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
	SpikeState state_;

	//移動量
	Vector3 velocity_;

	//減速量
	const float gensoku = 1.0f / 1000.0f;

	//死亡判定
	bool isDead_ = false;

	//爆破アニメーションのカウント
	int animationCount_ = 0;

	//アニメーションの終わりカウント
	const int maxAnimationCount = 60;



	//コリジョン処理するか否か
	bool collision_on = true;

	//無敵時間残りカウント
	int noCollisionCount_ = 0;

	//ボスに埋まり切るかのカウント
	int fillUpCount_;
	//埋まるまでのマックス
	const int maxFillUpCount_ = 6000;
	//埋まり切ったかのフラグ
	bool CompleteFillUp_ = false;


	//棘にかかる重力
	float gravity=-0.1f;

	//飛ぶ向きが左
	bool veloLeft_;
	//吹っ飛ぶときの加算量
	const float addVeloX_ = 1.0f / 30.0f;

	//飛ぶ時間
	float flyAwayCount_ = 0;
	//飛ぶ瞬間の最大カウント
	const float maxFlyAwayCount_ = 30;

};
