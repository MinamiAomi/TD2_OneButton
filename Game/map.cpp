#include "map.h"
#include<cassert>
#include<stdio.h>
#include<Externals/ImGui/imgui.h>





void Map::Initialize() { 
	
#pragma region 壁関連
	//壁のX値設定
	//最小値	
	Wall_min.translate = { -tileWide / 2,0,0 };
	//最大値
	Wall_max.translate = { (tileWide * mapTileNumX) - tileWide / 2.0f,0,0 };

	Wall_min.parent = &mapWorld_;
	Wall_max.parent = &mapWorld_;

#pragma endregion

	
#pragma region それ以外マップオブジェクト
	mapWorld_.translate = { -10.0f, 0.0f, 0.0f };
	mapWorld_.UpdateMatrix();

	//マップタイルによる座標設定
	for (int tileY = 0; tileY < mapTileNumY; tileY++) {
		for (int tileX = 0; tileX < mapTileNumX; tileX++) {
			if (mapTile[tileY][tileX] == Spike) {
				Transform world;
				world.translate = { tileWide * tileX, -tileWide * tileY, 0 };
				world.parent = &mapWorld_;
				world_.push_back(world);
			}
			//プレイヤー取得
			if (mapTile[tileY][tileX] == Player) {
				playerW_.translate = { tileWide * tileX, -tileWide * tileY, 0 };
				playerW_.parent = &mapWorld_;
				playerW_.UpdateMatrix();
			}

			if (mapTile[tileY][tileX] == Boss) {
				bossW_.translate = { tileWide * tileX, -tileWide * tileY, 0 };
				bossW_.parent = &mapWorld_;
				bossW_.UpdateMatrix();
			}
		}
	}


	
#pragma endregion


	//引き渡すので更新
	
	


	

	


}

void Map::Update() {
	ImGui::Begin("map");
	ImGui::DragFloat3("pos", &mapWorld_.translate.x, 0.01f);
	ImGui::End();

	hitsWallX_.x = Wall_min.worldMatrix.m[3][0];
	hitsWallX_.y = Wall_max.worldMatrix.m[3][0];


	mapWorld_.UpdateMatrix();
	
	Wall_min.UpdateMatrix();
	Wall_max.UpdateMatrix();
}




bool Map::IsHitWall(const Vector3& playerpos,const float& wide) {
	
	//
	if (playerpos.x - wide < GetWallMinX()) {
		return true;
	}

	if (playerpos.x + wide > GetWallMaxX()) {
		return true;
	}
	

	//外に出ていなければfalse
	return false;
}


