#include "Player.h"
#include"Externals/ImGui/imgui.h"

Player::Player() {}
Player::~Player() {}

void Player::Initalize(const Vector3& position, std::shared_ptr<ToonModel> PlayertoonModel) {
	


	//プレイヤーモデル受け取り
	model_ = PlayertoonModel;

	modelInstance_.SetModel(model_);

	
	
	//レーザーのモデル
	/*leser_model = new Model();
	leser_model = Model::Create();*/
	//プレイヤーのモデル
	worldTransform_.translate = position;

	input = Input::GetInstance();

	
}

void Player::Update() {
	ImGui::Begin("player");
	ImGui::DragFloat3("pos", &worldTransform_.translate.x, 0.01f);
	ImGui::Checkbox("isMove", &isMove);
	ImGui::End();

	//当たり判定するかの処理
	if (!collision_on) {
		if (noCollisionCount_-- <= 0) {
			collision_on = true;
		}
	}


	//各挙動初期化処理
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitalize();
			break;
		case Behavior::kJump:
			BehaviorJumpInitalize();
			break;
		case Behavior::kDrop:
			BehaviorDropInitalize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	//更新処理
	if (isMove) {
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootUpdate();
			break;
		case Behavior::kJump:
			BehaviorJumpUpdate();
			break;
		case Behavior::kDrop:
			BehaviorDropUpdate();
			break;
		}
	}
	//TODO
	/*for (Leser* leser : lesers_) {
		leser->Update();
	}*/

	//爆弾座標設定
	explosionPos_ = worldTransform_.translate;


	worldTransform_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(worldTransform_.worldMatrix);
}



void Player::OnCollision() {
	if (DropFlag) {
		return;
	}
	else {
		HP -= 1;
	}

}

void Player::OnCollisionWall(Vector2 wallX)
{
	if (wallX.x > worldTransform_.translate.x - wide_) {
		worldTransform_.translate.x = wallX.x + wide_;
	}
	if (wallX.y < worldTransform_.translate.x + wide_) {
		worldTransform_.translate.x = wallX.y - wide_;
	}
	worldTransform_.UpdateMatrix();

	moveXaxisSpeed *= -1;
}

void Player::OnCollisionBoss()
{
	isMove = false;
}

void Player::BehaviorRootInitalize() {
	behavior_ = Behavior::kRoot;
	DropFlag = false;
}

void Player::BehaviorRootUpdate() {
	worldTransform_.translate.y -= gravity;
	worldTransform_.translate.x += moveXaxisSpeed;
	//TODO
	/*lesers_.remove_if([](Leser* leser) {
		if (leser->GetIsAlive()) {
			delete leser;
			return true;
		}
		return false;
		});*/
	//スペースを押すとジャンプする
	if (input->GetInstance()->IsKeyTrigger(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kJump;
	}
}

void Player::BehaviorJumpInitalize() {
	behavior_ = Behavior::kJump;
	Jumpforce = 2.0f;
	moveXaxisSpeed *= -1;
	//TODO : Leserクラスを作成
	//Leser* leser_ = new Leser();
	//leser_->Initalize(leser_model, worldTransform_);
	//lesers_.push_back(leser_);
	DropCount = 0;
	DropFlag = false;
}

void Player::BehaviorJumpUpdate() {

	worldTransform_.translate.y += Jumpforce;
	worldTransform_.translate.x += moveXaxisSpeed;
	if (Jumpforce > 0) {
		Jumpforce -= gravity;
	}
	else if (Jumpforce <= 0) {
		Jumpforce = -kXaxisSpeed;
	}

	DropCount++;
	if (input->IsKeyTrigger(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kJump;
	}
	else if (
		DropCount == kDropAnime && input->IsKeyPressed(DIK_SPACE) != 0) {
		behaviorRequest_ = Behavior::kDrop;
	}
}

void Player::BehaviorDropInitalize() {
	behavior_ = Behavior::kDrop;
}

void Player::BehaviorDropUpdate() {
	worldTransform_.translate.y += 0.03f;
	DropCount++;
	if (DropCount >= kDropAnime * 3) {
		Attack();
	}
	if (input->IsKeyRelease(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorHitInitalize() {
	behavior_ = Behavior::kHit;
	//t = 0.0f;
}

void Player::BehaviorHitUpdate() {
	//t += 0.1f;
	//TODO : 敵に当たった時にLerpで画面上まで移動
	//worldTransform_.translation_.y = Lerp();
	
	/*if (t <= 1.0f) {
		behaviorRequest_ = Behavior::kRoot;
	}*/
}

void Player::Attack() {
	worldTransform_.translate.y -= 2.0f;
	DropFlag = true;
}



