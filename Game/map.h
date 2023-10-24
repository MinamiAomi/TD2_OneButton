#pragma once

#include "Math/Transform.h"

#include<string>

#include<vector>


struct hitWideX {
	float minX;
	float minY;
};

class Map {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 値設定
	/// </summary>
	void ValueSetting();


	/// <summary>
	/// 壁に当たっているかの確認
	/// </summary>
	/// <param name="playerpos">プレイヤーxy座標</param>
	/// <param name="wide">プレイヤーの半径</param>
	/// <returns>当たっているか否か</returns>
	bool IsHitWall(const Vector3& playerpos, const float& wide);

	/// <summary>
	/// 棘専用
	/// </summary>
	/// <param name="playerpos"></param>
	/// <param name="wide"></param>
	/// <returns></returns>
	bool IsHitWallSpike(const Vector3& playerpos, const float& wide);


	/// <summary>
	/// マップの移動速度を加速させる
	/// </summary>
	/// <param name="second">何秒処理するか</param>
	void SetMapMoveAcceleration(const float second);

#pragma region ゲッター

	//棘のtransform取得
	std::vector<Transform> GetSpikeWorld() { return world_; }

	//プレイヤーのmatworld取得
	Vector3 GetPlayerPosition() {
		return {
			playerW_.worldMatrix.m[3][0],
			playerW_.worldMatrix.m[3][1],
			playerW_.worldMatrix.m[3][2],

		};
	}

	//matWorld取得
	Vector3 GetBossMatPos() {
		return {
		bossW_.worldMatrix.m[3][0],
		bossW_.worldMatrix.m[3][1],
		bossW_.worldMatrix.m[3][2]
		};
	}

	//壁の左右のx座標取得(x=min,y=max)
	Vector2 GetHitWallX() { return hitsWallX_; }

	const Transform& GetEndTrans() { return mapEndTrans_; }
#pragma endregion


private:

	//マップ全体のワールド
	Transform mapWorld_;

	//マップチップ別のワールド
	std::vector<Transform> world_;

	//プレイヤー
	Transform playerW_;

	//ボス
	Transform bossW_;

#pragma region マップの配置関連
	//タイルサイズ
	static const int mapTileNumX_ = 15;
	static const int mapTileNumY_ = 32;

	//蒲田の四角のサイズ
	const float tileWide_ = 2.07f;

	//タイルの種類
	enum Tile {
		None,		//0
		Spike,		//1
		Player,		//2
		Boss		//3
	};

	//マップ情報
	const int mapTile_[mapTileNumY_][mapTileNumX_] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0},

	};

#pragma endregion
	
#pragma region 壁の処理関連
	//壁の左右のX値（x=min.y=max)
	Vector2 hitsWallX_;

	//壁との当たり判定で使う
	Transform Wall_min_;
	Transform Wall_max_;
#pragma endregion


#pragma region マップの移動関連
	//マップ加算量
	float moveMapNum_ = 0.01f;

	//マップが動くか
	bool mapMove_ = true;

#pragma region 加速処理関連
	//加速しているか否か
	bool isMoveAcceleration_ = false;

	//マップの加算量
	float mapAcceNum_ = 10.0f;

	//移動する時間
	float moveSeconds_;

#pragma endregion

#pragma endregion

	std::string groupName_ = "Map";

	Transform mapEndTrans_;
};