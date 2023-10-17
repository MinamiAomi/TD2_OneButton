#include"ImGame.h"

#include"Externals/ImGui/imgui.h"
#include "Engine/Graphics/RenderManager.h"
#include"Engine/Graphics/ModelLoader.h"
#include "Engine/Scene/SceneManager.h"


#include "Math/Transform.h"

void InGame::OnInitialize()
{

	RenderManager::GetInstance()->SetCamera(camera_);

	//カメラ座標初期化
	Vector3 camerapos = { 0.0f,0.0f,-50.0f };
	camera_.SetPosition(camerapos);

#pragma region モデルの初期化

	//仮モデル初期化
	toonModel_ = std::make_shared<ToonModel>();
	toonModel_->Create(ModelData::LoadObjFile("Resources/Model/sphere.obj"));

	
	/*
	//プレイヤーモデル
	playerModel_ = std::make_shared<ToonModel>();
	playerModel_->Create(ModelData::LoadObjFile("Resource/Model/player/player.obj"));
	//レーザー
	lezerModel_ = std::make_shared<ToonModel>();
	lezerModel_->Create(ModelData::LoadObjFile("Resource/Model/lazer/lazer.obj"));
	//レーザー爆発
	playerExplotionModel_ = std::make_shared<ToonModel>();
	playerExplotionModel_->Create(ModelData::LoadObjFile("Resource/Model/explosion/explosion.obj"));
	
	//プレイヤーモデルたち
	std::vector<std::shared_ptr<ToonModel>> playerModels_ = { playerModel_,lezerModel_,playerExplotionModel_ };

	//ボス
	bossModel_ = std::make_shared<ToonModel>();
	bossModel_->Create(ModelData::LoadObjFile("Resource/Model/boss/boss.obj"));
	//ボスの棘
	bossSpikeModel_ = std::make_shared<ToonModel>();
	bossSpikeModel_->Create(ModelData::LoadObjFile("Resource/Model/boss/boss.obj"));

	//ボスモデルたち
	std::vector<std::shared_ptr<ToonModel>>bossModels = { bossModel_,bossSpikeModel_ };

	//棘
	spikeModel_ = std::make_shared<ToonModel>();
	spikeModel_->Create(ModelData::LoadObjFile("Resource/Model/spike/spike.obj"));
	*/

#pragma endregion


	
	

	
	//マップクラス初期化
	map = std::make_unique<Map>();
	map->Initialize();

	//スパイクのTransformコピー
	std::vector<Transform> spikeWorld = map->GetSpikeWorld();
	//棘の数取得
	int spileNum = (int)spikeWorld.size();
	//棘の初期設定
	for (int num = 0; num < spileNum; num++) {
		//管理番号取得
		int sizeNum=(int)spikes.size();
		//クラス作成
		Spike* spike_ = new Spike();
		spike_->Initialize(sizeNum,spikeWorld[num], toonModel_,0);
		//プッシュ
		spikes.push_back(spike_);
	}
	//ボスの初期化
	boss_ = std::make_unique<Boss>();
	boss_->Initalize(map->GetBossMatPos(), toonModel_);

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initalize(map->GetPlayerPosition(),toonModel_);
	player_->SetBossY(&boss_->GetBossYLine());
	
}

void InGame::OnUpdate()
{
	
	

	//マップ更新
	map->Update();

	//ボス更新
	boss_->Update();
	

	//棘更新
	for (Spike* spike : spikes) {
		spike->Update();
	}

	//プレイヤー更新
	player_->Update();



	
	
	//当たり判定チェック
	GetAllCollisions();
	//
	CheckDead();


	//プレイヤー更新後にカメラ更新
	Vector3 cpos = boss_->GetmatWT();
	cpos.z = camera_.GetPosition().z;
	cpos.x = camera_.GetPosition().x;
	cpos.y += 20;
	camera_.SetPosition(cpos);
	
	camera_.UpdateMatrices();

}


bool CheckHitSphere(Vector3 p1, float w1, Vector3 p2, float w2) {

	Vector3 p = p1 - p2;

	float Length = sqrtf(p.x * p.x + p.y * p.y);
	//計算の誤差を許容する
	Length += 0.001f;

	if (Length < w1 + w2) {
		return true;
	}
	else {
		return false;
	}
}



void InGame::GetAllCollisions() {

	//プレイヤー座標と半径
	Vector3 PLAYER = player_->GetmatWtranslate();
	float P_wide = player_->GetWide();

#pragma region 棘に関する当たり判定
	

	//スパイクのWorld
	for (Spike* spike : spikes) {
		if (spike->GetIsCollisionOn()) {
			//座標と半径取得
			Vector3 SPIKE = spike->GetmatWtranstate();
			float S_wide = spike->GetWide();


#pragma region プレイヤー
			//当たった時の処理
			if (CheckHitSphere(SPIKE, S_wide, PLAYER, P_wide)) {
				spike->OnCollisionPlayer();
				player_->OnCollision();
			}
#pragma endregion

#pragma region プレイヤービームと爆風
			for (Leser* leser : player_->Getlesers()) {

				//レーザーのwide取得
				float beamWide = leser->GetWide();

				//ビームの終点取得
				Vector3 beamEnd = leser->GetExplosionPos();
				//高さを合わせる
				beamEnd.y = SPIKE.y;


				//ビームに当たっているとき
				if (PLAYER.y > beamEnd.y && CheckHitSphere(SPIKE, S_wide, beamEnd, beamWide)) {

					//一個目作成
					//サイズ取得
					int spikesize = (int)spikes.size();

					//新しいTransform作成
					Transform Newworld;
					//座標設定
					Newworld.translate = SPIKE;

					//最初に渡すベクトル値
					Vector3 newVelo = { 0.5f,0.5f,0 };

					//クラス作成
					Spike* newSpike = new Spike;
					newSpike->Initialize(spikesize, Newworld, toonModel_, Spike::SpikeState::kFalling,newVelo);
					//プッシュ
					spikes.push_back(newSpike);

					//二個目作成
					//x軸反転
					newVelo.x *= -1;

					Spike* newSpike2 = new Spike;
					newSpike2->Initialize(spikesize, Newworld, toonModel_, Spike::SpikeState::kFalling, newVelo);
					spikes.push_back(newSpike2);


					//死亡判定出す
					spike->SetDead();
					continue;
				}
				else {
					//ビーム当たっていない
					//爆風の範囲の場合
#pragma region プレイヤービーム爆風
					//爆風の座標を取得
					Vector3 ExpPos = leser->GetExplosionPos();
					float ExpWide = leser->GetWide();
					//爆風に当たった時
					if (CheckHitSphere(SPIKE, S_wide, ExpPos, ExpWide)) {
						spike->OnCollisionPlayerExplosion(ExpPos);
					}
#pragma endregion
				}
			}
			
				



			
#pragma endregion



#pragma region BOSS
			//埋まる状態でないときに処理
			if (!spike->IsStateFillUp()) {
				if (boss_->IsHitBoss(SPIKE, S_wide)) {
					spike->OnCollisionBoss();
				}
			}
#pragma endregion

#pragma region 壁
			//壁に当たっていれば処理
			if (map->IsHitWall(SPIKE, S_wide)) {
				spike->OnCollisionWall();
			}
#pragma endregion

#pragma region 棘同士

			
			int alredy = spike->GetIdentificationNum();

			int count = 0;
			for (Spike* spike2 : spikes) {

				if (count != alredy) {
					count++;
					continue;
				}


				//同じ棘でないことを確認
				if (spike->GetIdentificationNum() != spike2->GetIdentificationNum()) {

					//当たり判定処理がONになっていることの確認
					if (spike2->GetIsCollisionOn()) {
						Vector3 SPIKE2 = spike2->GetmatWtranstate();
						float S2_wide = spike2->GetWide();


						//当たり合ってたら処理
						if (CheckHitSphere(SPIKE, S_wide, SPIKE2, S2_wide)) {

							//二つの円の間の距離取得
							Vector3 leng = (SPIKE - SPIKE2);
							leng /= 2;
							leng = SPIKE2 + leng;

							//新しい円の半径設定
							float newSize = (S_wide + S2_wide) * 0.8f;

							//新しいTransform作成
							Transform newSpike;
							newSpike.translate = leng;							//位置設定
							newSpike.scale = { newSize,newSize ,newSize };		//サイズ設定

							//サイズ取得
							int sizeNum = (int)spikes.size();

							//新しいスパイクの生成
							Spike* newspike = new Spike();
							newspike->Initialize(sizeNum, newSpike, toonModel_, Spike::SpikeState::kFalling);

							//ぷっす
							spikes.push_back(newspike);

							//くっついた二つに死亡判定を
							spike->SetDead();
							spike2->SetDead();
						}



					}
				}
			}
			
#pragma endregion


#pragma region ボス回復処理
			//埋まり切りフラグがONの時回復
			if (spike->GetCompleteFillUp()) {
				boss_->OnCollisionHealing();
			}
#pragma endregion
		}
	}
#pragma endregion

#pragma region プレイヤーと壁
	if (map->IsHitWall(PLAYER, P_wide)) {
		player_->OnCollisionWall(map->GetHitWallX());
	}
#pragma endregion

#pragma region プレイヤーとボス
	if (boss_->IsHitBoss(PLAYER, P_wide)) {
		player_->OnCollisionBoss();
	}
#pragma endregion




	
}

void InGame::CheckDead() {
	spikes.remove_if([](Spike* spike) {
		if (spike->IsDead()) {
			delete spike;
			spike = nullptr;
			return true;
		}
		return false;
		});
}




void InGame::OnFinalize()
{
	for (Spike* spike : spikes) {
		delete spike;
		spike = nullptr;
	}

}