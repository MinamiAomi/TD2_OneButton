#pragma once
#include"Engine/Scene/BaseScene.h"
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Graphics/Sprite.h"
#include "Math/Camera.h"

#include<list>

#include"Spike.h"
#include"map.h"
#include"Player.h"
#include"Boss.h"

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
	/// 死亡判定
	/// </summary>
	void CheckDead();


private://メンバ関数
	
	//棘の当たり判定まとめ
	void CollisionAboutSpike();
	
private:

#pragma region 各種モデル
	//仮モデル
	std::shared_ptr<ToonModel> toonModel_;


	
	//ボスモデル
	//std::shared_ptr<ToonModel> bossModel_;

	//ボスが攻撃で使う棘モデル
	//std::shared_ptr<ToonModel>bossSpikeModel_;

	//棘モデル
	//std::shared_ptr<ToonModel> spikeModel_;


	
#pragma endregion


	//カメラ
	Camera camera_;

	

	//マップクラス
	std::unique_ptr<Map>map = nullptr;
	
	//棘クラス
	std::list<std::unique_ptr<Spike>> spikes;

	//プレイヤークラス
	std::unique_ptr<Player>player_ = nullptr;

	//ボスクラス
	std::unique_ptr<Boss>boss_ = nullptr;
};