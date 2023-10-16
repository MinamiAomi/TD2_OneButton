#pragma once
#pragma once
//
#include <optional>
//Engine
//#include "En"
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Math/Transform.h"
#include "Engine/Input/Input.h"
//
//#include "Leser.h"

enum class Behavior {
	kRoot,//通常
	kJump,//ジャンプ(単押し)
	kDrop,//落下攻撃(長押し)
	kHit,//ボスに当たった時
};

class Player {
public:
	Player();
	~Player();

	//初期化
	void Initalize(const Vector3& position, std::shared_ptr<ToonModel> toonModel);
	
	//更新
	void Update();
	
#pragma region オンコリジョン
	void OnCollision();

	//壁に当たった時
	void OnCollisionWall(Vector2 wallX);
#pragma endregion

#pragma region ゲッター
	const Vector3 GetPosition()
	{
		return worldTransform_.translate;
	}

	const Vector3 GetmatWtranslate() {
		return {
			worldTransform_.worldMatrix.m[3][0],
			worldTransform_.worldMatrix.m[3][1],
			worldTransform_.worldMatrix.m[3][2],
		};

	}
	const float GetWide() { return wide_; }
#pragma endregion

	

private:

	void BehaviorRootInitalize();
	void BehaviorRootUpdate();
	void BehaviorJumpInitalize();
	void BehaviorJumpUpdate();
	void BehaviorDropInitalize();
	void BehaviorDropUpdate();
	void BehaviorHitInitalize();
	void BehaviorHitUpdate();

	void Attack();

	//kamataEngine
	std::shared_ptr<ToonModel> model_ = nullptr;
	Transform worldTransform_;
	Input* input = nullptr;
	//
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//ジャンプ関連関数
	float gravity = 0.2f;
	float Jumpforce = 4.0f;
	//左右移動関係関数
	const float kXaxisSpeed = 0.20f;
	float moveXaxisSpeed = kXaxisSpeed;
	//落下攻撃関係関数
	int DropCount = 0;
	const int kDropAnime = 20;
	bool DropFlag = false;
	//ボスに当たった時用


	//std::list<Leser*> lesers_;
	//Model* leser_model;

	//TODO ちゃんといじろう
	int HP = 10;




private://スペチャ追加分
	float wide_ = 1;
	//モデル用
	ToonModelInstance modelInstance_;
	//移動関数
	bool isMove = false;
	//コリジョン処理するか否か
	bool collision_on = true;

	//無敵時間残りカウント
	int noCollisionCount_ = 0;
};
