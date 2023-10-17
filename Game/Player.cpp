#include "Player.h"
#include"Externals/ImGui/imgui.h"

Player::Player() {}
Player::~Player() {

	//削除
	for (Leser* leser : lesers_) {
		delete leser;
		leser = nullptr;
	}

}

void Player::Initalize(const Vector3& position, std::shared_ptr<ToonModel> PlayertoonModel) {
	
	//プレイヤーモデル受け取り
	model_ = PlayertoonModel;
	modelInstance_.SetModel(model_);

	//レーザー用モデル読み取り
	leser_model_ = PlayertoonModel;
	
	
	

	//プレイヤーのモデル
	worldTransform_.translate = position;

	input = Input::GetInstance();


	

}

void Player::Initialize(const Vector3& position, std::vector<std::shared_ptr<ToonModel>> models)
{
	//プレイヤーモデルの設定
	modelInstance_.SetModel(models[0]);

	//攻撃に使うモデルまとめ
	std::vector<std::shared_ptr<ToonModel>>ATKmodels = { models[1],models[2] };
	ATKmodels_ = ATKmodels;

	//プレイヤーのモデル
	worldTransform_.translate = position;

	//キー入力のインスタンス取得
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

	//爆弾座標設定
	explosionPos_ = worldTransform_.translate;


	worldTransform_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(worldTransform_.worldMatrix);

	
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
	if (input->IsKeyTrigger(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kJump;
	}
}

void Player::BehaviorJumpInitalize() {
	//状態を更新
	behavior_ = Behavior::kJump;
	//ジャンプ量を設定
	Jumpforce = 2.0f;
	//ｘ移動軸を反転
	moveXaxisSpeed *= -1;
	
#pragma region レーザー作成
	//レーザーの作成
	//プレイヤー座標取得
	Vector3 Ppos = GetmatWtranslate();
	//終点作成
	Vector3 Epos = Ppos;
	Epos.y =*BossY_;

	Leser* leser_ = new Leser();
	leser_->Initalize(leser_model_, Ppos,Epos );
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
	if (Jumpforce > 0) {
		Jumpforce -= gravity;
	}
	//ジャンプの値を左右移動と同じ速度にする
	else if (Jumpforce <= 0) {
		Jumpforce = -kXaxisSpeed;
	}
	//長押ししているとプラスされる
	DropCount++;
	//一度離してから再入力でもう一度ジャンプ
	if (input->IsKeyTrigger(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kJump;
	}
	//長押ししていたら落下へ
	else if (
		DropCount == kDropAnime && input->IsKeyPressed(DIK_SPACE) != 0) {
		behaviorRequest_ = Behavior::kDrop;
	}
}

void Player::BehaviorDropInitalize() {
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

void Player::BehaviorHitEnemyInitalize()
{
	behavior_ = Behavior::kHit;
	t_ = 0.0f;
	PposY = worldTransform_.translate.y;
}

void Player::BehaviorHitEnemyUpdate()
{
	t_ += 0.1f;
	//TODO : 敵に当たった時にLerpで画面上まで移動
	worldTransform_.translate.y = Math::Lerp(t_,PposY,EposY);

	if (t_ >= 1.0f) {
		behaviorRequest_ = Behavior::kRoot;
	}
}



