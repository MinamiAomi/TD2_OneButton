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
	//モデル
	modelInstance_.SetModel(toonModel);


	//状態入力
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
		animationCount_++;
		//アニメーションカウントがmaxの値で死亡
		if (maxAnimationCount <= animationCount_) {
			isDead_ = true;
		}

		break;
	default:
		break;
	}

	velocity_ = velo;
}

void Spike::Update() {

	

	switch (state_) {
	case Spike::kStay:
		break;
	case Spike::kFalling:

		//移動量
		world_.translate = world_.translate + velocity_;

		break;
	case Spike::kFillUp:
		world_.translate = world_.translate + (gensoku * velocity_);
		break;

	case Spike::kExplosion:

		break;

	case Spike::kNone:

		break;
	default:
		break;
	}

	world_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(world_.worldMatrix);
}



#pragma region OnCollision

void Spike::OnCollisionPlayer() { isDead_ = true; }

void Spike::OnCollisionBoss() {
	// 状態をゆっくり沈む状態へ
	state_ = kFillUp;
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
	state_ = kFalling;

	if (ExpPos.x < world_.translate.x) {
		velocity_ = { -1.0f, 0.0f, 0.0f };
	}
	else {
		velocity_ = { -1.0f, 0.0f, 0.0f };
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