#include "ImGame.h"

#include "Externals/ImGui/imgui.h"
#include "Engine/Graphics/RenderManager.h"
#include "Math/Transform.h"
#include"GlobalVariables.h"
#include "Clear.h"

void InGame::OnInitialize() {

	GlobalVariables::GetInstance()->LoadFiles();

	input_ = Input::GetInstance();

	RenderManager::GetInstance()->SetCamera(camera_);


	//カメラ座標初期化
	Vector3 camerapos = { 0.0f,0.0f,-40.0f };
	camera_.SetPosition(camerapos);










	//マップクラス初期化
	map = std::make_unique<Map>();
	map->Initialize();

	background_ = std::make_unique<Background>();
	background_->Initialize(&camera_);

	//ボスの初期化
	boss_ = std::make_unique<Boss>();
	boss_->Initalize(map->GetBossMatPos());

	//ヒールエフェクト


	//スパイクのTransformコピー
	std::vector<Transform> spikeWorld = map->GetSpikeWorld();
	//棘の数取得
	int AllSpikeNum = (int)spikeWorld.size();
	//棘の初期設定
	for (int num = 0; num < AllSpikeNum; num++) {

		AddSpike(spikeWorld[num]);


	}

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(map->GetPlayerPosition());
	player_->SetBossY(&boss_->GetBossYLine());


}

void InGame::OnUpdate() {
	GlobalVariables::GetInstance()->Update();


	//マップ更新
	map->Update();

	//ボス更新
	boss_->Update();


	//棘更新
	for (std::unique_ptr<Spike>& spike : spikes) {
		spike->Update();
	}

	//エフェクト更新
	for (Heal* heal_ : heals_) {
		heal_->Update();
	}
	//フラグが立っていたら削除
	heals_.remove_if([](Heal* Heal_) {
		if (Heal_->GetisAlive() == false) {
			delete Heal_;
			return true;
		}
		return false;
		});

	//プレイヤー更新
	player_->Update();



	//当たり判定チェック
	GetAllCollisions();
	//死亡チェック
	CheckDead();


#ifdef _DEBUG
	static float fovY = 25.0f;
	static float nearZ = 50.0f;
	static float farZ = 200.0f;
	static Vector3 position = { 0.0f, -44.0f, -100.0f };
	static Vector3 rotate = {};

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
	// 背景はカメラを使用しているためカメラの後に更新
	background_->Update();

	//シーンチェンジ処理
	SceneChange();
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

	//ボスに攻撃して埋まってるやつ全部爆破させたのでfalse
	if (player_->GetIsATKBossFlag()) {
		player_->SetATKBossFlag(false);
	}

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
			if (spike->GetIsCollisonOnPlayer() && CheckHitSphere(SPIKE, S_wide, PLAYER, P_wide)) {
				spike->OnCollisionPlayer();
				player_->OnCollision();
			}
			else {
				//プレイヤーの攻撃の爆破に巻き込まれた場合
				float playerExplotionWide = player_->GetExplosionRadius();

				//プレイヤーが攻撃したフラグON＆＆爆破半径内にある＆棘の状態が埋まる
				if (player_->GetIsATKBossFlag() && CheckHitSphere(SPIKE, S_wide, PLAYER, playerExplotionWide) && spike->IsStateFillUp()) {
					spike->OnCollisionPlayerStump();
				}
			}
#pragma endregion


#pragma region プレイヤービームと爆風
			//	プレイヤー攻撃に当たる状態かチェック

			for (Leser* leser : player_->Getlesers()) {
				if (spike->GetIsCollisonOnPlayer()) {
#pragma region ビーム
					if (!leser->IsAlreadyHit(spike->GetIdentificationNum())) {
						//レーザーのwide取得
						float beamWide = leser->GetLeserWide();

						//ビームの終点取得
						Vector3 beamEnd = leser->GetExplosionPos();
						//高さを合わせる
						beamEnd.y = SPIKE.y;


						//ビームに当たっているとき
						if (PLAYER.y > beamEnd.y && CheckHitSphere(SPIKE, S_wide, beamEnd, beamWide)) {

							leser->OnCollision(spike->GetIdentificationNum());

							//一個目作成
							float newWide = S_wide * 0.8f;

							//新しいTransform作成
							Transform Newworld;
							//座標設定
							Newworld.translate = SPIKE;
							Newworld.scale = { newWide,newWide,newWide };


							//最初に渡すベクトル値
							Vector3 newVelo = { 0.5f,0.5f,0 };


							AddSpike(Newworld, Spike::State::kFalling, newVelo);
							leser->OnCollision(spikeNum_);


							//二個目作成
							//x軸反転
							newVelo.x *= -1;

							AddSpike(Newworld, Spike::State::kFalling, newVelo);
							leser->OnCollision(spikeNum_);

							//死亡判定出す
							spike->SetDead();
							//死んだので処理を流す
							break;
						}
					}
#pragma endregion			

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

#pragma region 棘同士			
			//棘同士の当たり判定処理をするか&棘が木にくっついたままか
			if (spike->GetIsCollisonOnSpike() && !spike->IsStateStay()) {

				for (std::unique_ptr<Spike>& spike2 : spikes) {
					//棘が木にくっついていないことをチェック
					if (!spike2->IsStateStay()) {
						//同じ棘でないことを確認&&死んだ奴は処理しない
						if (spike->GetIdentificationNum() != spike2->GetIdentificationNum() && !spike2->IsDead()) {
							//棘座標取得
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

								//ダメージのついか
								int DMG = spike->GetDamege() + spike2->GetDamege();

								//ダメージの追加加算
								DMG += 1;

								//新しいスパイクの生成
								AddSpike(newSpike, Spike::State::kFalling, { 0.0f,0.0f,0.0f }, DMG);





								//オンコリ処理
								spike->OnCollisionSpike();
								spike2->OnCollisionSpike();

								//死んでるのでbreak
								break;
							}




						}
					}
				}
			}
#pragma endregion

#pragma region 壁
			//壁に当たっていれば処理
			if (map->IsHitWall(SPIKE, S_wide)) {
				spike->OnCollisionWall();
			}
#pragma endregion

#pragma region BOSS
			//ボスに対して当たり判定処理をするかどうかチェック
			if (spike->GetIsCollisionOnBoss()) {
				if (boss_->IsHitBoss(SPIKE, S_wide)) {
					spike->OnCollisionBoss();
				}
			}

			//爆破時の処理
			if (spike->IsDamageProcessing()) {
				if (boss_->IsHitBoss(SPIKE, S_wide)) {
					spike->OnCollisionExplotionBoss();
					boss_->OnCollisionExplosion(spike->GetDamege());
				}
			}
#pragma endregion

		}
#pragma region ボス回復処理
		//埋まり切りフラグがONの時回復
		if (spike->GetCompleteFillUp()) {
			boss_->OnCollisionHealing(spike->GetDamege());
			//ボスが回復するときのエフェクトを生成
			Heal* heal_ = new Heal();
			heal_->Initalize(spike->GetWorld().translate.GetXY());
			heals_.push_back(heal_);
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

void InGame::AddSpike(const Transform& trans, const int state, const Vector3 velo, int damage) {

	//クラス作成
	Spike* spike_ = new Spike();
	spike_->Initialize(spikeNum_, trans, &boss_->GetBossYLine(), damage, state, velo);
	//プッシュ
	spikes.emplace_back(spike_);

	//次の番号に変更
	spikeNum_++;

}


void InGame::SceneChange() {
	//1キーでクリア
	if (input_->IsKeyTrigger(DIK_1)) {
		//インスタンス取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		//シーン設定
		sceneManager->ChangeScene<Clear>();

	}
}

//終了処理
void InGame::OnFinalize() {

}



