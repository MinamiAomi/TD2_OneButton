#pragma once
#include"Engine/Scene/BaseScene.h"

#include "Engine/Graphics/ToonModel.h"
#include "Engine/Graphics/Sprite.h"
#include "Math/Camera.h"
#include"Engine/Input/Input.h"
#include<list>

#include"Spike.h"
#include"map.h"
#include"Player.h"
#include"Boss.h"
#include "Background.h"
#include"Limit.h"
#include "SpeedEffect.h"
#include "Heal.h"
#include "LeserDust.h"
#include "SpikeDust.h"


class InGame : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;



	/// <summary>
	/// すべての当たり判定処理まとめ
	/// </summary>
	void GetAllCollisions();

	/// <summary>
	/// シーンチェンジに関する処理
	/// </summary>
	void SceneChange();

	/// <summary>
	/// 残りのlimit計測
	/// </summary>
	void MapLimit();

	/// <summary>
	/// 死亡判定
	/// </summary>
	void CheckDead();

	/// <summary>
	/// 棘を増やす処理まとめ
	/// </summary>
	void AddSpike(const Transform& trans, const int state = 0, const Vector3 velo = { 0.0f,0.0f,0.0f },int damage=1);

private://メンバ関数

	//棘の当たり判定まとめ
	void CollisionAboutSpike();

private:

#pragma region 各種モデル



	//仮モデル
	std::shared_ptr<ToonModel> toonModel_;





#pragma endregion



	//キー入力
	Input* input_;


	//カメラ
	Camera camera_;

	

	//マップクラス
	std::unique_ptr<Map>map = nullptr;
	// 背景
	std::unique_ptr<Background> background_;
	// スピードエフェクト
	std::unique_ptr<SpeedEffect> speedEffect_;
	//棘クラス
	std::list<std::unique_ptr<Spike>> spikes;

	//棘の合計数と管理に使う
	int spikeNum_ = 0;

	//プレイヤークラス
	std::unique_ptr<Player>player_ = nullptr;

	//プレイヤーの攻撃でのダメージ
	int playerLeserDMG = 1;

	//ボスクラス
	std::unique_ptr<Boss>boss_ = nullptr;

	//エフェクト
	//ボスの回復
	std::list<std::unique_ptr<Heal>> heals_;
	//レーザーとボスが当たった時
	std::list<std::unique_ptr<LeserDust>> leserDusts_;
	//とげが壊れるとき
	std::list<std::unique_ptr<SpikeDust>> spikeDusts_;

	//リミットクラス
	std::unique_ptr<Limit>limit_ = nullptr;

	//吹き飛ばすベクトル量
	Vector3 Skipvelo = { 0.0f,2.0f,0.0 };

	//まっぷを加速させる秒数
	float mapAcceSecond_ = 2;

	//残りｍ数
	int limitScore_ = 0;


	

};