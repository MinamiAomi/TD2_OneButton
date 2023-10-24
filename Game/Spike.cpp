#include"Spike.h"
#include<cassert>

#include"Engine/Graphics/ModelLoader.h"
#include"Externals/ImGui/imgui.h"
#include "Graphics/ResourceManager.h"


void Spike::Initialize(int num, Transform world, const float* bossYLine, int DMG, int State, Vector3 velo) {
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char spikeModelName[] = "Spike";

	//管理番号
	spikeNum_ = num;
	//座標
	world_ = world;
	wide_ = world_.scale.x;
	world_.UpdateMatrix();


	//モデル
	modelInstance_.SetModel(resourceManager->FindModel(spikeModelName));

	//爆発モデル
	const char explosionModelName[] = "Explosion";
	exploModel_.SetModel(resourceManager->FindModel(explosionModelName));
	exploModel_.SetIsActive(false);
	exploTrans_.scale = { 0.0f,0.0f,0.0f };

	//ボスのY座標取得
	BossYLine_ = bossYLine;

	damage_ = DMG;


	//生成時すぐ当たらないよう処理
	collisionOnForBoss_ = false;
	collisionOnForPlayer_ = false;
	collisionOnForSpike_ = false;

	//当たらないフレーム数
	noCollisionCount_ = 15;


	//生成時の状態設定
	switch (State) {
	case kStay:
		state_ = kStay;
		break;
	case kFalling:
		state_ = kFalling;
		break;
	case kFillUp:
		state_ = kFillUp;
		break;
	case kExplosion:
		state_ = kExplosion;
		break;
	default:
		break;
	}

	velocity_ = velo;
}

void Spike::Update() {

	//状態が変化した際初期化処理をする
	CheckAllStateInitialize();

	//状態ごとの更新
	StateUpdate();


}

void Spike::CheckAllStateInitialize() {
	if (ckeckStateChange_) {
		switch (state_) {
		case Spike::kStay:
			Stay_Initialize();
			break;
		case Spike::kFalling:
			Falling_Initialize();
			break;
		case Spike::kFillUp:
			FillUp_Initiaize();
			break;
		case Spike::kExplosion:
			Explosion_Initialize();
			break;
		case Spike::kFlyAway:
			FlyAway_Initialize();
			break;
		case Spike::kNone:
			break;
		default:
			break;
		}
		ckeckStateChange_ = false;
	}
}

void Spike::StateUpdate() {
	//コリジョン処理をするかのフラグ処理
	if (noCollisionCount_ >= 0) {
		if (noCollisionCount_-- <= 0) {
			collisionOnForBoss_ = true;
			collisionOnForPlayer_ = true;
			collisionOnForSpike_ = true;
		}
	}

	//実行処理
	switch (state_) {
	case Spike::kStay:
		Stay_Update();
		break;
	case Spike::kFalling://落っこちる

		Falling_Update();

		break;
	case Spike::kFillUp://埋まっていく

		FillUp_Update();

		break;
	case Spike::kExplosion://爆発

		Explosion_Update();

		break;
	case Spike::kFlyAway:

		FlyAway_Update();

		break;

	case Spike::kNone:
		break;
	default:
		break;
	}

	world_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(world_.worldMatrix);

	//更新
	exploTrans_.UpdateMatrix();
	exploModel_.SetWorldMatrix(exploTrans_.worldMatrix);

}


#pragma region 各状態移行時初期化処理



void Spike::Stay_Initialize() {

}

void Spike::Falling_Initialize() {
	
}

void Spike::FillUp_Initiaize() {

	// 座標をmatにする
	world_.translate = GetmatWtranstate();
	// 親子関係の削除
	world_.parent = nullptr;
	//更新処理
	world_.UpdateMatrix();

	world_.translate.y = *BossYLine_ + wide_;


	//埋まるまでのカウント初期化
	fillUpCount_ = 0;

	// 横の加速度を削除
	velocity_.x = 0;





}

void Spike::Explosion_Initialize() {
	collisionOnForBoss_ = false;
	collisionOnForPlayer_ = false;
	collisionOnForSpike_ = false;

	noCollisionCount_ = 10000;
	isExplosion_ = true;
	isApplicationDamage = false;

	exploModel_.SetIsActive(true);
	modelInstance_.SetIsActive(false);

	animationCount_ = 0;

	exploTrans_.translate = GetmatWtranstate();
}

void Spike::FlyAway_Initialize() {

}


#pragma endregion

#pragma region 各状態更新処理

void Spike::Stay_Update() {
}

void Spike::Falling_Update() {
	//移動量
	world_.translate = world_.translate + velocity_;

	velocity_.y += gravity;

	//yベクトルが-でコリジョンON
	if (velocity_.y <= -0.00001f) {
		collisionOnForBoss_ = true;
	}
	else {
		collisionOnForBoss_ = false;
	}
}

void Spike::FillUp_Update() {

	//常にボスとの当たり判定処理はオフ
	collisionOnForBoss_ = false;

	//座標計算
	world_.translate = world_.translate + (gensoku_ * velocity_);

	//埋まり切ったら処理
	if (fillUpCount_++ >= maxFillUpCount_) {
		//死亡フラグON
		isDead_ = true;
		//埋まり切りフラグON
		CompleteFillUp_ = true;
		//コリジョンを切る
		collisionOnForBoss_ = false;
		noCollisionCount_ = 60;
	}
}

void Spike::Explosion_Update() {

	animationCount_++;

	//半径
	float t = (float)animationCount_ / (float)maxAnimationCount;

	if (t >= 1.0f) {
		t = 1.0f;
	}
	
	float wide= Math::Lerp(t, 0, wide_);
	exploTrans_.scale = { wide,wide,wide };
	
	
	//アニメーションカウントがmaxの値で死亡
	if (maxAnimationCount*1.5f <= animationCount_) {
		isDead_ = true;
	}
}

void Spike::FlyAway_Update() {
	world_.translate = world_.translate + velocity_;

	//飛ぶ方向チェック
	if (veloLeft_) {
		velocity_.x += addVeloX_;
		if (velocity_.x >= 0.0f) {
			state_ = kFillUp;

			ckeckStateChange_ = true;
		}
	}
	else {
		velocity_.x -= addVeloX_;
		if (velocity_.x <= 0.0f) {
			state_ = kFillUp;
			ckeckStateChange_ = true;;
		}
	}

	if (flyAwayCount_++ >= maxFlyAwayCount_) {
		state_ = kFillUp;
		ckeckStateChange_ = true;
	}
}
#pragma endregion



#pragma region OnCollision

void Spike::OnCollisionPlayer() {
	state_ = kExplosion;
	ckeckStateChange_ = true;
}

void Spike::OnCollisionBoss() {



	// 状態をゆっくり沈む状態へ
	state_ = kFillUp;
	ckeckStateChange_ = true;



}

void Spike::OnCollisionPlayerBeam() {
	// 死亡フラグ
	isDead_ = true;
	// 座標をmatにする
	world_.translate = GetmatWtranstate();
	// 親子関係の削除
	world_.parent = nullptr;
	// 新たに左右に吹っ飛ぶスパイクをIngameで作成
}

void Spike::OnCollisionPlayerExplosion(Vector3 ExpPos) {
	// 状態変化
	state_ = kFlyAway;
	ckeckStateChange_ = true;

	//爆心地によるベクトルと情報の初期化
	if (ExpPos.x < world_.translate.x) {
		velocity_ = exploVec_;
		veloLeft_ = false;
	}
	else {
		velocity_ = exploVec_;
		velocity_.x *= -1;
		veloLeft_ = true;
	}
	//座標をウエイに
	world_.translate.y = *BossYLine_ + wide_;

}

void Spike::OnCollisionSpike() {

	//
	isDead_ = true;

	//
	collisionOnForBoss_ = false;

	// 座標をmatにする
	world_.translate = GetmatWtranstate();
	// 親子関係の削除
	world_.parent = nullptr;
}


void Spike::OnCollisionPlayerStump() {
	// 状態変化
	state_ = kExplosion;
	ckeckStateChange_ = true;

	// 座標をmatにする
	world_.translate = GetmatWtranstate();
	// 親子関係の削除
	world_.parent = nullptr;
	//更新
	world_.UpdateMatrix();
}


void Spike::OnCollisionWall() {
	isDead_ = true;
	state_ = kNone;
}
void Spike::OnCollisionExplotionBoss() {
	//ダメージを与えたという処理を有効に
	isApplicationDamage = true;
}
void Spike::OnCollisionBossATK(Vector3 velo) {
	velocity_ = velo;
	state_ = kFalling;

	//コリジョン処理ほぼOFF
	collisionOnForBoss_ = false;
	collisionOnForPlayer_ = false;
	collisionOnForSpike_ = false;

	//戻らないような秒数
	noCollisionCount_ = 600;
}
void Spike::OnCollisionBossATKExplosion() {
	if (!IsCollisionBossSpikeATK_) {
		IsCollisionBossSpikeATK_ = true;
		state_ = kExplosion;
		ckeckStateChange_ = true;
	}
}
#pragma endregion


