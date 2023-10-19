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
#include "Leser.h"

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
	void Initialize(const Vector3& position);

	//更新
	void Update();
	
#pragma region オンコリジョン
	void OnCollision();

	//壁に当たった時
	void OnCollisionWall(Vector2 wallX);

	//ボスに当たった時
	void OnCollisionBoss();
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

	const Behavior GetBehavior() { return behavior_; }
	
	const std::list<Leser*>Getlesers() { return lesers_; }

	
#pragma endregion

#pragma region セッター
	void SetBossY(const float* bossY) { BossY_ = bossY; }
#pragma endregion

	

private:

	//モデル更新
	void ModelsUpdate();
	//モデルの回転修正
	Quaternion FixModelRotate(const char* label, const int& bodyPartNumber);


	//kRotのInitalize
	void BehaviorRootInitalize();
	//kRotのUpdate
	void BehaviorRootUpdate();
	//kJumpのInitalize
	void BehaviorJumpInitalize();
	//kJumpのUpdate
	void BehaviorJumpUpdate();
	//kDropのInitalize
	void BehaviorDropInitalize();
	//kDropのUpdate
	void BehaviorDropUpdate();
	//落下処理
	void Attack();
	//敵(トゲやボス)に当たった時
	//kHitのInitalize
	void BehaviorHitEnemyInitalize();
	//kHitのUpdate
	void BehaviorHitEnemyUpdate();

	//model
	std::shared_ptr<ToonModel> model_ = nullptr;
	//Transform
	Transform worldTransform_;
	
	Input* input = nullptr;
	//状態(Behavior)の管理
	Behavior behavior_ = Behavior::kRoot;
	//状態(Behavior)のリクエストを管理
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//重力
	float gravity = 0.2f;

	//ジャンプする力
	float Jumpforce = 4.0f;

	//左右移動の速度(絶対値)
	const float kXaxisSpeed = 0.20f;

	//左右移動の速度(コード内で使用)
	float moveXaxisSpeed = kXaxisSpeed;

	//長押しを何秒しているかのカウント
	int DropCount = 0;
	//20フレーム長押ししていたら落下
	const int kDropAnime = 20;
	//落ちている途中かどうかのフラグ
	bool DropFlag = false;

	//敵に当たった時のLerp用の関数(処理t)
	float t_ = 0;
	//プレイヤーの位置記録
	float PposY;
	//目標の位置
	float EposY = -31.0f;

	//TODO ちゃんといじろう
	//プレイヤーのHP
	int HP = 10;




private://スペチャ追加分

	//パーツ
	enum Parts {
		//頭
		kHead,
		//身体
		kBody,
		//左手
		kLArm,
		//右手
		kRArm,
		//左足
		kLFoot,
		//右足
		kRFoot
	};

	const int  PartsNum = 6;

#pragma region 身体モデル
	//モデルたち
	ToonModelInstance models_[6];

	//パーツのワールドTたち
	Transform worlds_[6];
	
#pragma endregion
	
	//円も出る幅
	float wide_ = 1;
	
	//移動関数
	bool isMove = false;

	//コリジョン処理するか否か
	bool collision_on = true;

	//無敵時間残りカウント
	int noCollisionCount_ = 0;

	//レーザーと爆風の仮システム作成
	std::list<Leser*>lesers_;
	
	//ボスの接触平面Y座標
	const float* BossY_;


#pragma region 奏太へ

	// 部位の総数 + 全体 の回転をつかさどる
	Vector3 modelEuler[7] = {
		{0.0f, 0.0f, 0.0f},
		{ -90.0f,0.5f,0.0f },
		{ -80.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f }
	};

	//頭
	const Vector3 modelHeadPos = { 0.0f,0.5f,-0.4f };
	const Quaternion modelHeadRot = Quaternion::MakeFromEulerAngle(modelEuler[1] * Math::ToRadian);
	//身体
	const Vector3 modelBodyPos = { 0.0f,-0.75f,1.0f };
	const Quaternion modelBodyRot = Quaternion::MakeFromEulerAngle(modelEuler[2] *Math::ToRadian);
	//左腕
	const Vector3 modelLArmPos = { -1.7f,0.0f,0.0f };
	const Quaternion modelLArmRot = Quaternion::MakeFromEulerAngle(modelEuler[3] *Math::ToRadian);
	//右腕
	const Vector3 modelRArmPos = { 1.7f,0.0f,0.0f };
	const Quaternion modelRArmRot = Quaternion::MakeFromEulerAngle(modelEuler[4] *Math::ToRadian);
	//左足
	const Vector3 modelLFootPos = { -0.8f,-1.5f,2.0f };
	const Quaternion modelLFootRot = Quaternion::MakeFromEulerAngle(modelEuler[5] *Math::ToRadian);
	//右足
	const Vector3 modelRFootPos = { 0.8f,-1.5f,2.0f };
	const Quaternion modelRFootRot = Quaternion::MakeFromEulerAngle(modelEuler[6] *Math::ToRadian);

#pragma endregion


};
