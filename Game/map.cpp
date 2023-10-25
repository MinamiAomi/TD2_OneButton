#include "map.h"
#include<cassert>
#include<stdio.h>
#include<Externals/ImGui/imgui.h>
#include"GlobalVariables.h"

void Map::Initialize() { 
#pragma region 壁関連
	//壁のX値設定
	//最小値	
	Wall_min_.translate = { -tileWide_ / 2,0,0 };
	//最大値
	Wall_max_.translate = { (tileWide_ * (mapTileNumX_-1)) + (tileWide_ / 2.0f),0,0 };

	Wall_min_.parent = &mapWorld_;
	Wall_max_.parent = &mapWorld_;
#pragma endregion	
#pragma region それ以外マップオブジェクト
	mapWorld_.translate = { -14.5f, 0.0f, 0.0f };
	mapWorld_.UpdateMatrix();

	//マップタイルによる座標設定
	for (int tileY = 0; tileY < mapTileNumY_; tileY++) {
		for (int tileX = 0; tileX < mapTileNumX_; tileX++) {
			//棘
			if (mapTile_[tileY][tileX] == Spike) {
				Transform world;
				world.translate = { tileWide_ * tileX, -tileWide_ * tileY, 0 };
				world.parent = &mapWorld_;
				world_.push_back(world);
			}
			//プレイヤー取得
			if (mapTile_[tileY][tileX] == Player) {
				playerW_.translate = { tileWide_ * tileX, -tileWide_ * tileY, 0 };
				playerW_.parent = &mapWorld_;
				playerW_.UpdateMatrix();
			}
			//ボス
			if (mapTile_[tileY][tileX] == Boss) {
				bossW_.translate = { tileWide_ * tileX, -tileWide_ * tileY, 0 };
				bossW_.parent = &mapWorld_;
				bossW_.UpdateMatrix();
			}
		}
	}
	//更新
	mapWorld_.UpdateMatrix();
	Wall_min_.UpdateMatrix();
	Wall_max_.UpdateMatrix();

	//壁の再計算
	hitsWallX_.x = Wall_min_.worldMatrix.m[3][0];
	hitsWallX_.y = Wall_max_.worldMatrix.m[3][0];
	//マップの一番上のポジション
	mapEndTrans_.parent = &mapWorld_;
	mapEndTrans_.translate = { 0.0f,0.0f,0.0f };
	mapEndTrans_.UpdateMatrix();

	prevY_ = mapWorld_.translate.y;
#pragma endregion

	//パラメータのうけとり
	ValueSetting();

}
void Map::ValueSetting() {
	GlobalVariables* globaV = GlobalVariables::GetInstance();
	GlobalVariables::GetInstance()->CreateGroup(groupName_);

	std::string keyVelocityMoveMap = "veloMoveMapNum : マップの１フレームに動く量";

	globaV->AddItem(groupName_, keyVelocityMoveMap, moveMapNum_);
	moveMapNum_ = globaV->GetFloatvalue(groupName_, keyVelocityMoveMap);

	std::string keyMapAcceNum = "mapMoveAcceNum : マップの加速する倍数";
	globaV->AddItem(groupName_, keyMapAcceNum, mapAcceNum_);
	mapAcceNum_ = globaV->GetFloatvalue(groupName_, keyMapAcceNum);
}


void Map::Update() {
#ifdef _DEBUG
	ImGui::Begin("map");
	ImGui::DragFloat3("pos", &mapWorld_.translate.x, 0.01f);
	ImGui::DragFloat3("scale", &mapWorld_.scale.x, 0.01f);
	ImGui::Checkbox("move", &mapMove_);
	ImGui::End();
#endif // _DEBUG

	prevY_ = mapWorld_.translate.y;

	//マップが動くとき
	if (mapMove_) {
		//加速処理を行っているか否か
		if (isMoveAcceleration_) {
			//行っている場合
			if (moveSeconds_-- <= 0.0f) {
				isMoveAcceleration_ = false;
			}


			//マップ移動
			mapWorld_.translate.y -= moveMapNum_ * mapAcceNum_;
		}
		else {
			//行っていない場合

			//マップ移動
			mapWorld_.translate.y -= moveMapNum_;
		}
	}

	//更新
	mapWorld_.UpdateMatrix();	
	Wall_min_.UpdateMatrix();
	Wall_max_.UpdateMatrix();

	//壁の再計算
	hitsWallX_.x = Wall_min_.worldMatrix.m[3][0];
	hitsWallX_.y = Wall_max_.worldMatrix.m[3][0];

	mapEndTrans_.UpdateMatrix();
}






bool Map::IsHitWall(const Vector3& playerpos,const float& wide) {
	
	//壁の左に当たっている場合true
	if (playerpos.x - wide < hitsWallX_.x) {
		return true;
	}

	//壁の右に当たっている場合true
	if (playerpos.x + wide > hitsWallX_.y) {
		return true;
	}
	

	//外に出ていなければfalse
	return false;
}

bool Map::IsHitWallSpike(const Vector3& playerpos, const float& wide) {

	//壁の左に当たっている場合true
	if (playerpos.x + wide < hitsWallX_.x) {
		return true;
	}

	//壁の右に当たっている場合true
	if (playerpos.x - wide > hitsWallX_.y) {
		return true;
	}


	//外に出ていなければfalse
	return false;
}

void Map::SetMapMoveAcceleration(const float second)
{
	isMoveAcceleration_ = true;
	//動く時間を取得
	moveSeconds_ = second*60;
}


