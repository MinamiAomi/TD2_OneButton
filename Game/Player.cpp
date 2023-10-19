#include "Player.h"
#include"Externals/ImGui/imgui.h"

#include "Graphics/ResourceManager.h"

Player::Player() {}
Player::~Player() {

	//削除
	for (Leser* leser : lesers_) {
		delete leser;
		leser = nullptr;
	}

}



void Player::Initialize(const Vector3& position)
{
	//キー入力のインスタンス取得
	input = Input::GetInstance();

	const char* modelNames[] = {
		"PlayerHead",
		"PlayerBody",
		"PlayerLArm",
		"PlayerRArm",
		"PlayerLFoot",
		"PlayerRFoot",
	};

	ResourceManager* resourceManager = ResourceManager::GetInstance();
	//モデルセット
	for (int i = 0; i < PartsNum; i++) {
		models_[i].SetModel(resourceManager->FindModel(modelNames[i]));
	}

	//プレイヤーのモデル
	worldTransform_.translate = position;
	wide_ = worldTransform_.scale.x;

	//親設定
	for (int i = 0; i < PartsNum; i++) {
		worlds_[i].parent = &worldTransform_;
	}
#pragma region パーツの座標設定

	worlds_[kHead].translate = modelHeadPos;
	worlds_[kHead].rotate = modelHeadRot;

	worlds_[kBody].translate = modelBodyPos;
	worlds_[kBody].rotate = modelBodyRot;

	worlds_[kLArm].translate = modelLArmPos;
	worlds_[kLArm].rotate = modelLArmRot;

	worlds_[kRArm].translate = modelRArmPos;
	worlds_[kRArm].rotate = modelRArmRot;

	worlds_[kLFoot].translate = modelLFootPos;
	worlds_[kLFoot].rotate = modelLFootRot;

	worlds_[kRFoot].translate = modelRFootPos;
	worlds_[kRFoot].rotate = modelRFootRot;

#pragma endregion


}

void Player::Update() {
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("pos", &worldTransform_.translate.x, 0.01f);
	worldTransform_.rotate = FixModelRotate("rotate", 0);
	ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f);
	ImGui::Checkbox("isMove", &isMove);
	ImGui::DragInt("DropCount", &DropCount);
	ImGui::End();
#endif // _DEBUG


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
		case Behavior::kHit:
			BehaviorHitEnemyInitalize();
			break;
		}
		//リクエストのリセット
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
		case Behavior::kHit:
			BehaviorHitEnemyUpdate();
			break;
		}
	}
	//TODO
	for (Leser* leser : lesers_) {
		leser->Update();
	}



	//プレイヤー中心行列更新
	worldTransform_.UpdateMatrix();

	ModelsUpdate();


	lesers_.remove_if([](Leser* leser) {
		//レーザーが死んだら処理
		if (!leser->GetIsAlive()) {
			delete leser;
			leser = nullptr;
			return true;
		}
		return false;
		});

}
void Player::ModelsUpdate() {
#ifdef _DEBUG
	ImGui::Begin("player Parts");
	ImGui::Text("positions");
	ImGui::DragFloat3("head pos", &worlds_[kHead].translate.x, 0.01f);
	ImGui::DragFloat3("body pos", &worlds_[kBody].translate.x, 0.01f);

	ImGui::DragFloat3("LArm pos", &worlds_[kLArm].translate.x, 0.01f);
	ImGui::DragFloat3("RArm pos", &worlds_[kRArm].translate.x, 0.01f);

	ImGui::DragFloat3("LFoot pos", &worlds_[kLFoot].translate.x, 0.01f);
	ImGui::DragFloat3("RFoot pos", &worlds_[kRFoot].translate.x, 0.01f);

	ImGui::Text("rotates");

	worlds_[kHead].rotate = FixModelRotate("head rotate", 1);
	worlds_[kBody].rotate = FixModelRotate("body rotate", 2);
	worlds_[kLArm].rotate = FixModelRotate("LArm rotate", 3);
	worlds_[kRArm].rotate = FixModelRotate("RArm rotate", 4);
	worlds_[kLFoot].rotate = FixModelRotate("LFoot rotate", 5);
	worlds_[kRFoot].rotate = FixModelRotate("RFoot rotate", 6);

	ImGui::End();
#endif // _DEBUG



#pragma region モデル更新
	for (int i = 0; i < PartsNum; i++) {
		worlds_[i].UpdateMatrix();
		models_[i].SetWorldMatrix(worlds_[i].worldMatrix);
	}
#pragma endregion
}

Quaternion Player::FixModelRotate(const char* label, const int& bodyPartNumber) {
	ImGui::DragFloat3(label, &modelEuler[bodyPartNumber].x, 0.1f);
	modelEuler[bodyPartNumber].x = std::fmod(modelEuler[bodyPartNumber].x, 360.0f);
	modelEuler[bodyPartNumber].y = std::fmod(modelEuler[bodyPartNumber].y, 360.0f);
	modelEuler[bodyPartNumber].z = std::fmod(modelEuler[bodyPartNumber].z, 360.0f);
	return Quaternion::MakeFromEulerAngle(modelEuler[bodyPartNumber] * Math::ToRadian);
}

void Player::OnCollision() {
	if (DropFlag) {
		return;
	}
	else {
		HP -= 1;
	}

}

void Player::OnCollisionWall(Vector2 wallX) {
	if (wallX.x > worldTransform_.translate.x - wide_) {
		worldTransform_.translate.x = wallX.x + wide_;
	}
	if (wallX.y < worldTransform_.translate.x + wide_) {
		worldTransform_.translate.x = wallX.y - wide_;
	}
	worldTransform_.UpdateMatrix();

	//移動を反転
	MoveReverse();
}

void Player::OnCollisionBoss() {
	behaviorRequest_ = Behavior::kHit;
	//isMove = false;
}



void Player::BehaviorRootInitalize() {
	behavior_ = Behavior::kRoot;
	DropFlag = false;
}

void Player::BehaviorRootUpdate() {
	//毎フレームずつ下に引っ張られる
	worldTransform_.translate.y -= gravity;
	//左右移動
	worldTransform_.translate.x += moveXaxisSpeed;

	//スペースを押すとジャンプする
	if (input->IsKeyRelease(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kJump;
	}
}

void Player::BehaviorJumpInitalize() {
	//状態を更新
	behavior_ = Behavior::kJump;
	//ジャンプ量を設定
	Jumpforce = 2.0f;
	//移動を反転
	MoveReverse();

#pragma region レーザー作成
	//レーザーの作成
	//プレイヤー座標取得
	Vector3 Ppos = GetmatWtranslate();
	//終点作成
	Vector3 Epos = Ppos;
	Epos.y = *BossY_;

	Leser* leser_ = new Leser();
	leser_->Initialize(Ppos,Epos );
	lesers_.push_back(leser_);
#pragma endregion

	//ドロップカウントを初期化
	DropCount = 0;
	//ドロップフラグを初期化
	DropFlag = false;
}

void Player::BehaviorJumpUpdate() {
	//ジャンプの値をプラス
	worldTransform_.translate.y += Jumpforce;
	//左右移動
	worldTransform_.translate.x += moveXaxisSpeed;
	//ジャンプの値から毎フレームずつ重力を引いていく
	if (Jumpforce > -kXaxisSpeed) {
		Jumpforce -= gravity;
	}
	//ジャンプの値を左右移動と同じ速度にする
	else {
		Jumpforce = -kXaxisSpeed;
	}
	//一度離してから再入力でもう一度ジャンプ
	if (input->IsKeyRelease(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kJump;
	}
	//長押ししていたら落下へ
	else if (input->IsKeyPressed(DIK_SPACE) != 0) {
		//長押ししているとプラスされる
		DropCount++;
		if (DropCount == kDropAnime) {
			behaviorRequest_ = Behavior::kDrop;
		}
	}
}

void Player::BehaviorDropInitalize() {
	//落下用にモデルを調整する
	modelEuler[0] = { 90.0f, 180.0f, 0.0f };
	worldTransform_.rotate = Quaternion::MakeFromEulerAngle(modelEuler[0] * Math::ToRadian);
	modelEuler[1] = {-180.0f, modelEuler[1].y, 180.0f};
	worlds_[kHead].rotate = Quaternion::MakeFromEulerAngle(modelEuler[1] * Math::ToRadian);
	worlds_[kLFoot].translate = { worlds_[kLFoot].translate.x, -0.75f, worlds_[kLFoot].translate.z };
	worlds_[kRFoot].translate = { worlds_[kRFoot].translate.x, -0.75f, worlds_[kRFoot].translate.z };

	behavior_ = Behavior::kDrop;
}

void Player::BehaviorDropUpdate() {
	//貯めている時はゆっくり上に上がる
	worldTransform_.translate.y += 0.03f;
	//貯めている時にフレームをカウントする
	DropCount++;
	if (DropCount >= kDropAnime * 3) {
		//貯めているフレームが一定を超えたら、攻撃を開始する
		Attack();
	}

}

void Player::Attack() {
	//落下中の移動処理
	worldTransform_.translate.y -= 2.0f;
	//落ちているときにフラグが立つ
	DropFlag = true;
	/*if (ボスに当たったら) {
		behaviorRequest_ = Behavior::kHit;
	}*/
}

void Player::BehaviorHitEnemyInitalize() {
	behavior_ = Behavior::kHit;
	t_ = 0.0f;
	PposY = worldTransform_.translate.y;
}

void Player::BehaviorHitEnemyUpdate() {
	//t_に値を加算
	t_ += 0.1f;
	//Lerpで上まで動かす
	worldTransform_.translate.y = Math::Lerp(t_, PposY, EposY);
	//TODO : 上空で少し待機する処理を追加
	if (t_ >= 1.0f) {
		// モデルを "落下用" → "基本用" へ移行する
		modelEuler[0] = { 0.0f, 0.0f, 0.0f };
		worldTransform_.rotate = Quaternion::MakeFromEulerAngle(modelEuler[0] * Math::ToRadian);
		modelEuler[1] = { -90.0f, modelEuler[1].y, 0.0f };
		worlds_[kHead].rotate = Quaternion::MakeFromEulerAngle(modelEuler[1] * Math::ToRadian);
		worlds_[kLFoot].translate = { worlds_[kLFoot].translate.x, -1.5f, worlds_[kLFoot].translate.z };
		worlds_[kRFoot].translate = { worlds_[kRFoot].translate.x, -1.5f, worlds_[kRFoot].translate.z };

		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::MoveReverse() {
	//ｘ移動軸を反転
	moveXaxisSpeed *= -1;

	//ｘ移動の自然数判定で傾きを変化させる
	if (moveXaxisSpeed > 0) {
		modelEuler[0].z = -kGlideAngle;
	}
	else {
		modelEuler[0].z = kGlideAngle;
	}
	worldTransform_.rotate = Quaternion::MakeFromEulerAngle(modelEuler[0] *Math::ToRadian);
}
