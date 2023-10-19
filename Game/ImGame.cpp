#include "ImGame.h"

#include "Externals/ImGui/imgui.h"
#include "Engine/Graphics/RenderManager.h"
#include "Engine/Graphics/ModelLoader.h"
#include "Engine/Scene/SceneManager.h"


#include "Math/Transform.h"

void InGame::OnInitialize() {

    RenderManager::GetInstance()->SetCamera(camera_);

    //カメラ座標初期化
    Vector3 camerapos = { 0.0f,0.0f,-40.0f };
    camera_.SetPosition(camerapos);

#pragma region モデルの初期化

    //仮モデル初期化
 /*   toonModel_ = std::make_shared<ToonModel>();
    toonModel_->Create(ModelData::LoadObjFile("Resources/Model/sphere.obj"));*/

        /*
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

	E_BossHeal = std::make_shared<Texture>();
	E_BossHeal->Load("Resources/Heal.png");
	sprite.SetTexture(E_BossHeal);
	sprite.SetIsActive(true);
	sprite.SetPosition(TexPos);


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
        int sizeNum = (int)spikes.size();
        //クラス作成
        Spike* spike_ = new Spike();
        spike_->Initialize(sizeNum, spikeWorld[num], 0);
        //プッシュ
        spikes.emplace_back(spike_);
    }
    //ボスの初期化
    boss_ = std::make_unique<Boss>();
    boss_->Initalize(map->GetBossMatPos());

    //プレイヤーの初期化
    player_ = std::make_unique<Player>();

    player_->Initialize(map->GetPlayerPosition());

    player_->SetBossY(&boss_->GetBossYLine());

}

void InGame::OnUpdate() {



    //マップ更新
    map->Update();

    //ボス更新
    boss_->Update();


    //棘更新
    for (std::unique_ptr<Spike>& spike : spikes) {
        spike->Update();
    }

    //プレイヤー更新
    player_->Update();



    //当たり判定チェック
    GetAllCollisions();
    //
    CheckDead();


    //プレイヤー更新後にカメラ更新
    Vector3 cpos = boss_->GetMatWT();
    cpos.z = camera_.GetPosition().z;
    cpos.x = camera_.GetPosition().x;
    cpos.y += 20;
    camera_.SetPosition(cpos);

#ifdef _DEBUG
    static float fovY = 25.0f;
    static float nearZ = 50.0f;
    static float farZ = 200.0f;
    static Vector3 position = { 0.0f, -42.0f, -100.0f };
    static Vector3 rotate = {};

    ImGui::Begin("Texture");
    ImGui::DragFloat2("Position", &TexPos.x, 0.1f);
    sprite.SetPosition(TexPos);
    ImGui::End();
    //position = camera_.GetPosition();

    ImGui::Begin("Camera");
    ImGui::DragFloat3("Position", &position.x, 0.1f);
    ImGui::DragFloat3("Rotate", &rotate.x, 0.1f);
    ImGui::DragFloat("FovY", &fovY, 0.1f, 0.0f, 180.0f);
    ImGui::DragFloat("NearZ", &nearZ, 0.1f);
    ImGui::DragFloat("FarZ", &farZ, 1.0f);
    ImGui::End();

    rotate.x = std::fmod(rotate.x, 360.0f);
    rotate.y = std::fmod(rotate.y, 360.0f);
    rotate.z = std::fmod(rotate.z, 360.0f);

    camera_.SetPosition(position);
    camera_.SetRotate(Quaternion::MakeFromEulerAngle(rotate * Math::ToRadian));
    camera_.SetPerspective(fovY * Math::ToRadian, 540.0f / 720.0f, nearZ, farZ);
#endif // _DEBUG

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


    CollisionAboutSpike();

    //プレイヤー座標と半径
    Vector3 PLAYER = player_->GetmatWtranslate();
    float P_wide = player_->GetWide();
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



void InGame::CollisionAboutSpike() {
#pragma region 棘に関する当たり判定
    //スパイクのWorld

    //プレイヤー座標と半径
    Vector3 PLAYER = player_->GetmatWtranslate();
    float P_wide = player_->GetWide();


    for (std::unique_ptr<Spike>& spike : spikes) {
      //死んだ弾は処理しない
        	if (!spike->IsDead()) {
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
			//	プレイヤー攻撃に当たる状態かチェック
			if (spike->GetIsCollisonOnPlayer()) {
				for (Leser* leser : player_->Getlesers()) {
#pragma region ビーム
					//レーザーのwide取得
					float beamWide = leser->GetLeserWide();

					//ビームの終点取得
					Vector3 beamEnd = leser->GetExplosionPos();
					//高さを合わせる
					beamEnd.y = SPIKE.y;


					//ビームに当たっているとき
					if (PLAYER.y > beamEnd.y && CheckHitSphere(SPIKE, S_wide, beamEnd, beamWide)) {

						//一個目作成
						//サイズ取得
						int spikesize = (int)spikes.size();

						float newWide = S_wide * 0.8f;

						//新しいTransform作成
						Transform Newworld;
						//座標設定
						Newworld.translate = SPIKE;
						Newworld.scale = { newWide,newWide,newWide };

						//最初に渡すベクトル値
						Vector3 newVelo = { 0.5f,0.5f,0 };

						//クラス作成
						Spike* newSpike = new Spike;
						newSpike->Initialize(spikesize, Newworld, Spike::State::kFalling, newVelo);
						//プッシュ
						spikes.emplace_back(newSpike);

						//二個目作成
						//x軸反転
						newVelo.x *= -1;

						//クラス作成とプッシュ
						Spike* newSpike2 = new Spike;
						newSpike2->Initialize(spikesize, Newworld, Spike::State::kFalling, newVelo);
						spikes.emplace_back(newSpike2);


						//死亡判定出す
						spike->SetDead();
						//死んだので処理を流す
						continue;
#pragma endregion			
					}
					else {
						//ビーム当たっていない
						//爆風の範囲の場合
#pragma region プレイヤービーム爆風
					//爆風の座標を取得
						Vector3 ExpPos = leser->GetExplosionPos();
						float ExpWide = leser->GetExplotionRadius();
						//爆風に当たった時
						if (CheckHitSphere(SPIKE, S_wide, ExpPos, ExpWide)) {
							spike->OnCollisionPlayerExplosion(ExpPos);
						}
#pragma endregion
					}
				}
			}
#pragma endregion

#pragma region BOSS
			//ボスに対して当たり判定処理をするかどうかチェック
			if (spike->GetIsCollisionOnBoss()) {
				if (boss_->IsHitBoss(SPIKE, S_wide)) {
					spike->OnCollisionBoss(boss_->GetBossYLine());
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
			//棘同士の当たり判定処理をするか
			if (spike->GetIsCollisonOnSpike()) {
				 for (std::unique_ptr<Spike>& spike2 : spikes) {
					//同じ棘でないことを確認&&死んだ奴は処理しない
					if (spike->GetIdentificationNum() != spike2->GetIdentificationNum() && !spike->IsDead() && !spike2->IsDead()) {


						Vector3 SPIKE2 = spike2->GetmatWtranstate();
						float S2_wide = spike2->GetWide();

                        //当たり合ってたら処理
                        if (CheckHitSphere(SPIKE, S_wide, SPIKE2, S2_wide)) {

                            //二つの円の間の距離取得
                            Vector3 leng = (SPIKE - SPIKE2);
                            leng /= 2;
                            leng = SPIKE2 + leng;

                            //新しい円の半径設定
                            float newSize = (S_wide + S2_wide) * 0.6f;

                            //新しいTransform作成
                            Transform newSpike;
                            newSpike.translate = leng;							//位置設定
                            newSpike.scale = { newSize,newSize ,newSize };		//サイズ設定

                            //サイズ取得
                            int sizeNum = (int)spikes.size();

                            //新しいスパイクの生成
                            Spike* newspike = new Spike();
                            newspike->Initialize(sizeNum, newSpike, Spike::State::kFalling);

                            //ぷっす
                            spikes.emplace_back(newspike);


							//オンコリ処理
							spike->OnCollisionSpike();
							spike2->OnCollisionSpike();


							break;
						}




					}
				}
			}
#pragma endregion
		}
#pragma region ボス回復処理
            //埋まり切りフラグがONの時回復
            if (spike->GetCompleteFillUp()) {
                boss_->OnCollisionHealing();
            }
#pragma endregion

		
	}
#pragma endregion


}


//死亡チェック
void InGame::CheckDead() {
    spikes.remove_if([](std::unique_ptr<Spike>& spike) {
        if (spike->IsDead()) {
            return true;
        }
        return false;
        });
}

//終了処理
void InGame::OnFinalize() {

}