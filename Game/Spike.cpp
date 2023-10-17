#include"Spike.h"
#include<cassert>

#include"Engine/Graphics/ModelLoader.h"
#include"Externals/ImGui/imgui.h"


void Spike::Initialize(int num,Transform world, std::shared_ptr<ToonModel> toonModel, int State, Vector3 velo) {
	//管理番号
	spikeNum_=num;
	//座標
	world_ = world;	
	wide_ = world_.scale.x;
	world_.UpdateMatrix();


	//モデル
	modelInstance_.SetModel(toonModel);
	

	//状態入力
	switch (State) {
	case kStay:
		state_ = kStay;
		break;
	case kFalling:
		//生成時落下処理の時すぐ当たらないよう処理
		state_ = kFalling;
		collision_on = false;
		noCollisionCount_ = 15;


		break;
	case kFillUp:
		state_ = kFillUp;
		break;

	case kExplosion:
		
		break;
	default:
		break;
	}

	velocity_ = velo;
}

void Spike::Update() {

	//コリジョン処理をするかのフラグ処理
	if (!collision_on) {
		if (noCollisionCount_-- <= 0) {
			collision_on = true;
		}
	}
	
	//実行処理
	switch (state_) {
	case Spike::kStay:
		break;
	case Spike::kFalling://落っこちる

		//移動量
		world_.translate = world_.translate + velocity_;

		velocity_.y += gravity;

		//yベクトルが-でコリジョンON
		if (velocity_.y <= -0.00001f) {
			collision_on = true;
			noCollisionCount_ = 0;
		}
		break;
	case Spike::kFillUp://埋まっていく
		//座標計算
		world_.translate = world_.translate + (gensoku * velocity_);

		if (fillUpCount_++ >= maxFillUpCount_) {
			//死亡フラグON
			isDead_ = true;
			//埋まり切りフラグON
			CompleteFillUp_ = true;
			//コリジョンを切る
			collision_on = false;
			noCollisionCount_ = 60;
		}

		

		break;
	case Spike::kExplosion://爆発

		animationCount_++;
		//アニメーションカウントがmaxの値で死亡
		if (maxAnimationCount >= animationCount_) {
			isDead_ = true;
		}

		break;
	case Spike::kFlyAway:
		world_.translate = world_.translate + velocity_;

		//飛ぶ方向チェック
		if (veloLeft_) {
			velocity_.x += addVeloX_;
			if(velocity_.x>=0.0f){
				state_ = kFillUp;
				velocity_.x = 0;
				velocity_.y = 0;
			}
		}
		else {
			velocity_.x -= addVeloX_;
			if (velocity_.x <= 0.0f) {
				state_ = kFillUp;
				velocity_.x = 0;
				velocity_.y = 0;
			}
		}

		if (flyAwayCount_++ >= maxFlyAwayCount_) {
			state_ = kFillUp;
			velocity_.x = 0;
			velocity_.y = 0;
		}

		break;

	case Spike::kNone:

		break;
	default:
		break;
	}

	//行列更新
	world_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(world_.worldMatrix);
}



#pragma region OnCollision

void Spike::OnCollisionPlayer() { isDead_ = true; }

void Spike::OnCollisionBoss() {
	// 状態をゆっくり沈む状態へ
	state_ = kFillUp;

	//埋まるまでのカウント初期化
	fillUpCount_ = 0;

	// 横の加速度を削除
	velocity_.x = 0;

	// 座標をmatにする
	world_.translate = GetmatWtranstate();
	// 親子関係の削除
	world_.parent = nullptr;
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

	//爆心地によるベクトルと情報の初期化
	if (ExpPos.x < world_.translate.x) {
		velocity_ = exploVec;
		veloLeft_ = false;
	}
	else {
		velocity_ = exploVec;
		velocity_.x *= -1;
		veloLeft_ = true;
	}

	
}

void Spike::OnCollisionSpike() {
	// 座標をmatにする
	world_.translate = GetmatWtranstate();
	// 親子関係の削除
	world_.parent = nullptr;	
}


void Spike::OnCollisionPlayerStump() {
	// 状態変化
	state_ = kExplosion;

	// 座標をmatにする
	world_.translate= GetmatWtranstate();
	// 親子関係の削除
	world_.parent = nullptr;
}


void Spike::OnCollisionWall() {
	isDead_ = true;
	state_ = kNone;
}


#pragma endregion