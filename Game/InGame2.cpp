#include"InGame2.h"

#include"Engine/Scene/SceneManager.h"
#include "Externals/ImGui/imgui.h"
#include "Engine/Graphics/RenderManager.h"
#include "Math/Transform.h"
#include"GlobalVariables.h"
#include "Clear.h"

void InGame2::OnInitialize() {


	input_ = Input::GetInstance();

	RenderManager::GetInstance()->SetCamera(camera_);

	//カメラ座標初期化
	//カメラ初期化
	camera_.SetPosition({ 0.0f, -44.0f, -100.0f });
	camera_.SetRotate({});
	camera_.SetPerspective(25.0f * Math::ToRadian, 540.0f / 720.0f, 50.0f, 200.0f);
	camera_.UpdateMatrices();

	//マップクラス初期化
	map = std::make_unique<Map>();
	map->Initialize(false);

	background_ = std::make_unique<Background>();
	background_->Initialize(&camera_);

	speedEffect_ = std::make_unique<SpeedEffect>();
	speedEffect_->Initialize(&camera_);

	//ボスの初期化
	boss_ = std::make_unique<BossSecond>();
	boss_->Initalize(map->GetBossMatPos());


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


	limit_ = std::make_unique<Limit>();
	limit_->Initialize();
}

void InGame2::OnUpdate() {
	GlobalVariables::GetInstance()->Update();


	//最初の落下攻撃をするととおる
	if (player_->GetIsFirstAttack() == true) {
		//マップ更新
		map->Update();
		//ボス更新
		boss_->Update();
	}

	//棘更新
	for (std::unique_ptr<Spike>& spike : spikes) {
		spike->Update();
	}

	//エフェクト更新
	for (std::unique_ptr<Heal>& heal_ : heals_) {
		heal_->Update();
	}
	//フラグが立っていたら削除
	heals_.remove_if([](std::unique_ptr<Heal>& Heal_) {
		if (Heal_->GetisAlive() == false) {
			return true;
		}
		return false;
		});

	//プレイヤー更新
	player_->Update();



	//当たり判定チェック
	GetAllCollisions();

	//残りの距離取得
	MapLimit();

	//死亡チェック
	CheckDead();


	static float fovY = 25.0f;
	static float nearZ = 50.0f;
	static float farZ = 200.0f;
	static Vector3 position = { 0.0f, -44.0f, -100.0f };
	static Vector3 rotate = {};

#ifdef _DEBUG
	//position = camera_.GetPosition();
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Position", &position.x, 0.1f);
	ImGui::DragFloat3("Rotate", &rotate.x, 0.1f);
	ImGui::DragFloat("FovY", &fovY, 0.1f, 0.0f, 180.0f);
	ImGui::DragFloat("NearZ", &nearZ, 0.1f);
	ImGui::DragFloat("FarZ", &farZ, 1.0f);
	if (ImGui::Button("SpeedEffect")) {
		speedEffect_->Spawn();
	}
	ImGui::End();
#endif // _DEBUG

	rotate.x = std::fmod(rotate.x, 360.0f);
	rotate.y = std::fmod(rotate.y, 360.0f);
	rotate.z = std::fmod(rotate.z, 360.0f);
	camera_.SetPosition(position);
	camera_.SetRotate(Quaternion::MakeFromEulerAngle(rotate * Math::ToRadian));
	camera_.SetPerspective(fovY * Math::ToRadian, 540.0f / 720.0f, nearZ, farZ);

	camera_.UpdateMatrices();
	// 背景はカメラを使用しているためカメラの後に更新
	background_->Update();
	speedEffect_->Update();
	//シーンチェンジ処理
	SceneChange();
}


bool InGame2::CheckHitSphere(Vector3 p1, float w1, Vector3 p2, float w2) {

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



void InGame2::GetAllCollisions() {


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
		//ボスに攻撃していない＆＆最初の攻撃実行炭
		if (!player_->GetIsATKBossFlag() && player_->GetIsFirstAttack() == true) {
			map->SetMapMoveAcceleration(mapAcceSecond_);
		}
	}
	if (boss_->IsHitBossATK(PLAYER, P_wide)) {
		player_->OnCollisionBoss();
		if (player_->GetIsFirstAttack() == true) {
			map->SetMapMoveAcceleration(mapAcceSecond_);
		}
	}
#pragma endregion





}





void InGame2::CollisionAboutSpike() {
#pragma region 棘に関する当たり判定
	//スパイクのWorld

	//プレイヤー座標と半径
	Vector3 PLAYER = player_->GetmatWtranslate();
	float P_wide = player_->GetWide();


	for (std::unique_ptr<Spike>& spike : spikes) {
		//死んだ弾は処理しない
		if (!spike->IsDead()) {

#pragma region ボスの攻撃処理で使用されたとき
			if (boss_->GetBossATKSpikeExplo()) {
				//埋まっていく状態の敵をすべてコリジョンオフにして飛ばす
				if (spike->IsStateFillUp()) {
					spike->OnCollisionBossATK(Skipvelo);
				}
			}
#pragma endregion



			//座標と半径取得
			Vector3 SPIKE = spike->GetmatWtranstate();
			float S_wide = spike->GetWide();
#pragma region プレイヤー
			//当たった時の処理
			if (spike->GetIsCollisonOnPlayer() && CheckHitSphere(SPIKE, S_wide, PLAYER, P_wide)) {
				spike->OnCollisionPlayer();
				player_->OnCollision();
				//最初の落下攻撃をしていたら、マップの加速を許可
				if (player_->GetIsFirstAttack() == true) {
					map->SetMapMoveAcceleration(mapAcceSecond_);
				}
			}
			else {
				//プレイヤーが攻撃したフラグON＆＆爆破半径内にある＆棘の状態が埋まる
				if (player_->GetIsATKBossFlag() && spike->IsStateFillUp()) {
					spike->OnCollisionPlayerStump();
				}
			}
#pragma endregion


#pragma region プレイヤービームと爆風			
			for (Leser* leser : player_->Getlesers()) {
				//	プレイヤー攻撃に当たる状態かチェック
				if (spike->GetIsCollisonOnPlayer()) {
#pragma region ビーム

					//レーザーのwide取得
					float beamWide = leser->GetLeserWide();

					//ビームの終点取得
					Vector3 beamEnd = leser->GetExplosionPos();
					//高さを合わせる
					beamEnd.y = SPIKE.y;


					//ビームに当たっているとき
					if (PLAYER.y > beamEnd.y && CheckHitSphere(SPIKE, S_wide, beamEnd, beamWide)) {
						//同じレーザーが新しく生成した棘と当たらないようにする処理
						if (!leser->IsAlreadyHit(spike->GetIdentificationNum())) {

							leser->OnCollision(spike->GetIdentificationNum());

							//一個目作成
							float newWide = S_wide / 2.0f;

							//新しいTransform作成
							Transform Newworld;
							//座標設定
							Newworld.translate = SPIKE;
							Newworld.scale = { newWide,newWide,newWide };


							//最初に渡すベクトル値
							Vector3 newVelo = { 0.5f,0.5f,0 };



							AddSpike(Newworld, Spike::State::kFalling, newVelo, spike->GetCoalescenceCount());
							//生成した棘の番号登録
							leser->OnCollision(spikeNum_);


							//二個目作成
							//x軸反転
							newVelo.x *= -1;

							AddSpike(Newworld, Spike::State::kFalling, newVelo);
							//生成した棘の番号登録
							leser->OnCollision(spikeNum_);

							//死亡判定出す
							spike->SetDead();
							//死んだので処理を流す
							break;
						}

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
#pragma endregion			


				}
			}
#pragma endregion

#pragma region 棘同士			
			//棘同士の当たり判定処理をするか&棘が木にくっついたままか&&合体数が3回以下か
			if (spike->GetIsCollisonOnSpike() && !spike->IsStateStay() && spike->GetCoalescenceCount() < 3) {

				for (std::unique_ptr<Spike>& spike2 : spikes) {
					//棘が木にくっついていないことをチェック
					if (!spike2->IsStateStay() && spike2->GetCoalescenceCount() < 3) {
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
								float newSize = (S_wide + S2_wide);

								//新しいTransform作成
								Transform newSpike;
								newSpike.translate = leng;							//位置設定
								newSpike.scale = { newSize,newSize ,newSize };		//サイズ設定

								//合体数計算(DMGと動議
								int DMG = spike->GetCoalescenceCount() + spike2->GetCoalescenceCount();


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
			if (map->IsHitWallSpike(SPIKE, S_wide)) {
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


#pragma region ボスの攻撃で当たったかどうか
			if (boss_->IsHitBossATK(SPIKE, S_wide)) {
				spike->OnCollisionBossATKExplosion();
			}
#pragma endregion
		}
#pragma region ボス回復処理
		//埋まり切りフラグがONの時回復
		if (spike->GetCompleteFillUp()) {
			boss_->OnCollisionHealing(spike->GetDamege());
			//ボスが回復するときのエフェクトを生成
			Heal* heal_ = new Heal();
			heal_->Initalize({ spike->GetWorld().translate.GetXY() });
			heals_.emplace_back(heal_);
		}
#pragma endregion


	}
#pragma endregion


}


//死亡チェック
void InGame2::CheckDead() {
	spikes.remove_if([](std::unique_ptr<Spike>& spike) {
		if (spike->IsDead()) {
			return true;
		}
		return false;
		});
}

void InGame2::AddSpike(const Transform& trans, const int state, const Vector3 velo, int damage) {

	//クラス作成
	Spike* spike_ = new Spike();
	spike_->Initialize(spikeNum_, trans, &boss_->GetBossYLine(), damage, state, velo);
	//プッシュ
	spikes.emplace_back(spike_);

	//次の番号に変更
	spikeNum_++;

}


void InGame2::SceneChange() {
	//1キーでクリア
	if (input_->IsKeyTrigger(DIK_1)) {

#pragma region データ転送
		const char dataName[] = "data";
		//インスタンス取得
		GlobalVariables* globalV = GlobalVariables::GetInstance();
		//グループの追加
		GlobalVariables::GetInstance()->CreateGroup(dataName);

		//limit
		std::string keyLimit = "Limit";
		//値の登録
		globalV->AddItem(dataName, keyLimit, limitScore_);
		//ボブかどうか
		std::string IsBob = "IsBob";
		globalV->AddItem(dataName, IsBob, 0);
#pragma endregion



		//インスタンス取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		//シーン設定
		sceneManager->ChangeScene<Clear>();

	}
}

void InGame2::MapLimit() {
	float limitY = map->GetEndTrans().worldMatrix.m[3][1];

	float bossY = boss_->GetBossYLine();

	//残り計算（42は棘の終点が画面上に来た時にぴったり0になる数値
	limitScore_ = (int)limitY - (int)bossY - 41;

	//0以下は表示する必要なし
	if (limitScore_ <= 0.0f) {
		limitScore_ = 0;
	}

	limit_->Update(limitScore_);

#ifdef _DEBUG
	ImGui::Begin("limit");
	ImGui::Text("limit : %4.1f", limitScore_);
	ImGui::End();
#endif // _DEBUG

}

//終了処理
void InGame2::OnFinalize() {

}



