#pragma once
#include"Engine/Scene/BaseScene.h"
#include"Engine/Scene/SceneManager.h"
#include "Engine/Graphics/ToonModel.h"
#include "Engine/Graphics/Sprite.h"
#include "Math/Camera.h"
#include"Engine/Input/Input.h"
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
	/// シーンチェンジに関する処理
	/// </summary>
	void SceneChange();


	/// <summary>
	/// 死亡判定
	/// </summary>
	void CheckDead();

	/// <summary>
	/// 棘を増やす処理まとめ
	/// </summary>
	void AddSpike(const Transform& trans, const int state = 0, const Vector3 velo = { 0.0f,0.0f,0.0f });

private://メンバ関数

	//棘の当たり判定まとめ
	void CollisionAboutSpike();

private:

#pragma region 各種モデル



	//仮モデル
	std::shared_ptr<ToonModel> toonModel_;





#pragma endregion


#pragma region シーン転換
	//キー入力
	Input* input_;

	// シーンマネージャー
	SceneManager* sceneManager_;
#pragma endregion


	//カメラ
	Camera camera_;

	std::shared_ptr<Texture>E_BossHeal;
	Sprite sprite;
	//Vector2 TexPos = {0,0};
	Transform TexPos_;

	//マップクラス
	std::unique_ptr<Map>map = nullptr;

	//棘クラス
	std::list<std::unique_ptr<Spike>> spikes;

	//棘の合計数と管理に使う
	int spikeNum_ = 0;

	//プレイヤークラス
	std::unique_ptr<Player>player_ = nullptr;

	//ボスクラス
	std::unique_ptr<Boss>boss_ = nullptr;
};