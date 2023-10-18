#pragma once


#include<vector>

#include "Math/Transform.h"


struct hitWideX {
	float minX;
	float minY;
};

class Map {
public:
	void Initialize();

	void Update();

	//void Draw(ViewProjection view);

	/// <summary>
	/// 壁に当たっているかの確認
	/// </summary>
	/// <param name="playerpos">プレイヤーxy座標</param>
	/// <param name="wide">プレイヤーの半径</param>
	/// <returns>当たっているか否か</returns>
	bool IsHitWall(const Vector3& playerpos, const float& wide);


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

	Vector3 GetBossMatPos() {
		return {
		bossW_.worldMatrix.m[3][0],
		bossW_.worldMatrix.m[3][1],
		bossW_.worldMatrix.m[3][2]
		};
	}



	Vector2 GetHitWallX() { return hitsWallX_; }
#pragma endregion


private:

	float GetWallMinX() { return Wall_min.worldMatrix.m[3][0]; }
	float GetWallMaxX() { return Wall_max.worldMatrix.m[3][0]; }

	Vector2 hitsWallX_;

	//タイルサイズ
	static const int mapTileNumX = 13;
	static const int mapTileNumY = 32;

	//蒲田の四角のサイズ
	const float tileWide = 2.0f;

	enum Tile {
		None,
		Spike,
		Player,
		Boss
	};

	int wallCount = 0;

	const int mapTile[mapTileNumY][mapTileNumX] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1,1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
		{0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0,0},
	};



	//タイルの合計数
	const int tileNum = mapTileNumX * mapTileNumY;

	struct WallX {
		float min;
		float max;
	};

	//壁との当たり判定で使う
	Transform Wall_min;
	Transform Wall_max;



	//マップ全体のワールド
	Transform mapWorld_;

	//マップチップ別のワールド
	std::vector<Transform> world_;

	//プレイヤー
	Transform playerW_;


	//ボス
	Transform bossW_;


	//マップ配置データ
	std::vector<std::vector<int>> mapData_;


	//マップ加算量
	const float moveMapNum_ = 0.01f;


	bool mapMove_ = true;
};