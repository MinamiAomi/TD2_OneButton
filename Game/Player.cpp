#include "Player.h"

#ifdef _DEBUG
#include"Externals/ImGui/imgui.h"
#endif // _DEBUG

#include "Graphics/ResourceManager.h"
#include"GlobalVariables.h"

Player::Player() {}
Player::~Player() {



	//削除
	for (Leser* leser : lesers_) {
		delete leser;
		leser = nullptr;
	}

}



void Player::Initialize(const Vector3& position) {
	//キー入力のインスタンス取得
	input_ = Input::GetInstance();

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


	//親設定
	for (int i = 0; i < PartsNum; i++) {
		worlds_[i].parent = &worldTransform_;
	}


	ValueSetting();



}

void Player::ValueSetting() {

#pragma region パラメータ設定
	//ファイルのキー名変えたりするとおかしくなってフォルダ内のjson消さなきゃいけなくなるのでこれは残す
	
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
	
	//インスタンス取得
	GlobalVariables* globalV = GlobalVariables::GetInstance();
	//グループの追加
	GlobalVariables::GetInstance()->CreateGroup(groupName_);


#pragma region parts



	std::string keyHead = "head : 頭";
	std::string keyBody = "body : 身体";
	std::string keyLArm = "LeftArm : 左腕";
	std::string keyRArm = "RightArm : 右腕";
	std::string keyLFoot = "LeftFoot : 左足";
	std::string keyRFoot = "RightFoot : 右足";


	//情報登録
	globalV->AddItem(groupName_, keyHead + "座標", worlds_[kHead].translate);
	globalV->AddItem(groupName_, keyBody + "座標", worlds_[kBody].translate);
	globalV->AddItem(groupName_, keyLArm + "座標", worlds_[kLArm].translate);
	globalV->AddItem(groupName_, keyRArm + "座標", worlds_[kRArm].translate);
	globalV->AddItem(groupName_, keyLFoot + "座標", worlds_[kLFoot].translate);
	globalV->AddItem(groupName_, keyRFoot + "座標", worlds_[kRFoot].translate);

	//setData
	worlds_[kHead].translate = globalV->GetVector3value(groupName_, keyHead + "座標");
	worlds_[kBody].translate = globalV->GetVector3value(groupName_, keyBody + "座標");
	worlds_[kLArm].translate = globalV->GetVector3value(groupName_, keyLArm + "座標");
	worlds_[kRArm].translate = globalV->GetVector3value(groupName_, keyRArm + "座標");
	worlds_[kLFoot].translate = globalV->GetVector3value(groupName_, keyLFoot + "座標");
	worlds_[kRFoot].translate = globalV->GetVector3value(groupName_, keyRFoot + "座標");

	//回転むりぽ
#pragma endregion

#pragma region Other

	//各種キー
	std::string keyWide = "wide : 半径";
	std::string keyHp = "HP : ヒットポイント";
	std::string keyGravity = "gravity : 重力";
	std::string keyJumpForce = "jumpForce : ジャンプパワー";
	std::string keykXaxisSPD = "x-axisSPD : 横移動最大速度";
	std::string keyDropF = "DropATKCountFrame : フレーム数長押しで落下";
	std::string keyGlideAngle = "glideAngle : 移動時の傾き量";


	globalV->AddItem(groupName_, keyWide, wide_);
	globalV->AddItem(groupName_, keyHp, HP_);
	globalV->AddItem(groupName_, keyGravity, gravity_);
	globalV->AddItem(groupName_, keyJumpForce, Jumpforce_);
	globalV->AddItem(groupName_, keykXaxisSPD, kXaxisSpeed_);
	globalV->AddItem(groupName_, keyDropF, kDropAnime_);
	globalV->AddItem(groupName_, keyGlideAngle, kGlideAngle_);

	wide_ = globalV->GetFloatvalue(groupName_, keyWide);
	HP_ = globalV->GetIntvalue(groupName_, keyHp);
	gravity_ = globalV->GetFloatvalue(groupName_, keyGravity);
	Jumpforce_ = globalV->GetFloatvalue(groupName_, keyJumpForce);
	kXaxisSpeed_ = globalV->GetFloatvalue(groupName_, keykXaxisSPD);
	kDropAnime_ = globalV->GetIntvalue(groupName_, keyDropF);
	kGlideAngle_ = globalV->GetFloatvalue(groupName_, keyGlideAngle);
#pragma endregion
#pragma endregion


}


void Player::Update() {
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("pos", &worldTransform_.translate.x, 0.01f);
	worldTransform_.rotate = FixModelRotate("rotate", 0);
	ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f);
	ImGui::Checkbox("isMove", &isMove_);
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
	if (isMove_) {
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

	ImGui::End();
#endif // _DEBUG

	worlds_[kHead].rotate = FixModelRotate("head rotate", 1);
	worlds_[kBody].rotate = FixModelRotate("body rotate", 2);
	worlds_[kLArm].rotate = FixModelRotate("LArm rotate", 3);
	worlds_[kRArm].rotate = FixModelRotate("RArm rotate", 4);
	worlds_[kLFoot].rotate = FixModelRotate("LFoot rotate", 5);
	worlds_[kRFoot].rotate = FixModelRotate("RFoot rotate", 6);


#pragma region モデル更新
	for (int i = 0; i < PartsNum; i++) {
		worlds_[i].UpdateMatrix();
		models_[i].SetWorldMatrix(worlds_[i].worldMatrix);
	}
#pragma endregion
}

Quaternion Player::FixModelRotate(const char* label, const int& bodyPartNumber) {
	label;
#ifdef _DEBUG
	ImGui::DragFloat3(label, &modelEuler[bodyPartNumber].x, 0.1f);
#endif // _DEBUG
	label;
	modelEuler[bodyPartNumber].x = std::fmod(modelEuler[bodyPartNumber].x, 360.0f);
	modelEuler[bodyPartNumber].y = std::fmod(modelEuler[bodyPartNumber].y, 360.0f);
	modelEuler[bodyPartNumber].z = std::fmod(modelEuler[bodyPartNumber].z, 360.0f);
	return Quaternion::MakeFromEulerAngle(modelEuler[bodyPartNumber] * Math::ToRadian);
}


void Player::OnCollision() {

	//処理状態遷移
	behaviorRequest_ = Behavior::kHit;
	if (DropFlag) {
		return;
	}
	else {
		HP_ -= 1;
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

	//長押し攻撃で当たった場合
	if (behavior_ == Behavior::kDrop) {
		isATKBoss_ = true;
	}//その他の場合
	else {
		
	}
	//処理状態遷移
	behaviorRequest_ = Behavior::kHit;
	
	//isMove = false;
}



void Player::BehaviorRootInitalize() {
	behavior_ = Behavior::kRoot;
	DropFlag = false;
}

void Player::BehaviorRootUpdate() {
	//毎フレームずつ下に引っ張られる
	worldTransform_.translate.y -= gravity_;
	//左右移動
	worldTransform_.translate.x += moveXaxisSpeed_;

	//スペースを押すとジャンプする
	if (input_->IsKeyRelease(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kJump;
	}
}

void Player::BehaviorJumpInitalize() {
	//状態を更新
	behavior_ = Behavior::kJump;
	//ジャンプ量を設定
	Jumpforce_ = 2.0f;
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
	leser_->Initialize(Ppos, Epos);
	lesers_.push_back(leser_);
#pragma endregion

	//ドロップカウントを初期化
	DropCount = 0;
	//ドロップフラグを初期化
	DropFlag = false;
}

void Player::BehaviorJumpUpdate() {
	//ジャンプの値をプラス
	worldTransform_.translate.y += Jumpforce_;
	//左右移動
	worldTransform_.translate.x += moveXaxisSpeed_;
	//ジャンプの値から毎フレームずつ重力を引いていく
	if (Jumpforce_ > -kXaxisSpeed_) {
		Jumpforce_ -= gravity_;
	}
	//ジャンプの値を左右移動と同じ速度にする
	else {
		Jumpforce_ = -kXaxisSpeed_;
	}
	//一度離してから再入力でもう一度ジャンプ
	if (input_->IsKeyRelease(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kJump;
	}
	//長押ししていたら落下へ
	else if (input_->IsKeyPressed(DIK_SPACE) != 0) {
		//長押ししているとプラスされる
		DropCount++;
		if (DropCount == kDropAnime_) {
			behaviorRequest_ = Behavior::kDrop;
		}
	}
}

void Player::BehaviorDropInitalize() {
	//落下用にモデルを調整する
	modelEuler[0] = { 90.0f, 180.0f, 0.0f };
	worldTransform_.rotate = Quaternion::MakeFromEulerAngle(modelEuler[0] * Math::ToRadian);
	modelEuler[1] = { 90.0f, modelEuler[1].y, 180.0f };
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
	if (DropCount >= kDropAnime_ * 3) {
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

	float remakeT = t_;
	if (remakeT >= 1.0f) {
		remakeT = 1.0f;
	}
	//Lerpで上まで動かす
	worldTransform_.translate.y = Math::Lerp(remakeT, PposY, EposY);
	//TODO : 上空で少し待機する処理を追加
	if (t_ >= 1.2f) {
		// モデルを "落下用" → "基本用" へ移行する
		modelEuler[0] = { 0.0f, 0.0f, 0.0f };
		worldTransform_.rotate = Quaternion::MakeFromEulerAngle(modelEuler[0] * Math::ToRadian);
		modelEuler[1] = { 0.0f, modelEuler[1].y, 0.0f };
		worlds_[kHead].rotate = Quaternion::MakeFromEulerAngle(modelEuler[1] * Math::ToRadian);
		worlds_[kLFoot].translate = { worlds_[kLFoot].translate.x, -1.5f, worlds_[kLFoot].translate.z };
		worlds_[kRFoot].translate = { worlds_[kRFoot].translate.x, -1.5f, worlds_[kRFoot].translate.z };

		behaviorRequest_ = Behavior::kRoot;

		//コリジョンするか
		isCollisonActive = true;
	}
}

void Player::MoveReverse() {
	//ｘ移動軸を反転
	moveXaxisSpeed_ *= -1;

	//ｘ移動の自然数判定で傾きを変化させる
	if (moveXaxisSpeed_ > 0) {
		modelEuler[0].z = -kGlideAngle_;
	}
	else {
		modelEuler[0].z = kGlideAngle_;
	}
	worldTransform_.rotate = Quaternion::MakeFromEulerAngle(modelEuler[0] * Math::ToRadian);
}
