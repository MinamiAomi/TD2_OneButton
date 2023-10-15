#include "map.h"
#include<cassert>
#include<stdio.h>
#include<Externals/ImGui/imgui.h>





void Map::Initialize() { 
	
	
	

	
	//マップタイルによる座標設定
	for (int tileY = 0; tileY < mapTileNumY; tileY++) {
		for (int tileX = 0; tileX < mapTileNumX; tileX++) {
			if (mapTile[tileY][tileX] == Spike) {
				Transform world;
				world.translate = {tileWide * tileX, -tileWide * tileY, 0};
				
				world.parent = &mapWorld_;
				world_.push_back(world);


				
			}
			

			//プレイヤー取得
			if (mapTile[tileY][tileX] == Player) {
				playerW_.translate = {tileWide * tileX, -tileWide * tileY, 0};
				
			}
		}
	}


	//壁のX値設定
	//最小値	
	Wall_min.translate = { tileWide * -1,0,0 };
	//最大値
	Wall_max.translate = { tileWide * mapTileNumX,0,0 };
	
	Wall_min.parent = &mapWorld_;
	Wall_max.parent = &mapWorld_;


	mapWorld_.translate = { -10.0f, 0.0f, 0.0f };

}

void Map::Update() {
	ImGui::Begin("map");
	ImGui::DragFloat3("pos", &mapWorld_.translate.x, 0.01f);
	ImGui::End();


	mapWorld_.UpdateMatrix();
	
	Wall_min.UpdateMatrix();
	Wall_max.UpdateMatrix();
}




bool Map::IsHitWall(const Vector3& playerpos,const float& wide) {
	if ((playerpos.x + wide) < GetWallMinX() - wide ) {
		return true;
	}

	if ((playerpos.x - wide) > GetWallMaxX() + wide) {
		return true;
	}
	

	//外に出ていなければfalse
	return false;
}


