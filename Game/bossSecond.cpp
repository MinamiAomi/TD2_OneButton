#include"bossSecond.h"

#include "Graphics/ResourceManager.h"
#include"Externals/ImGui/imgui.h"

#include"GlobalVariables.h"

void BossSecond::Initalize(const Vector3& position) {
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char bossModelName[] = "Boss2";

	//座標設定
	world_.translate = position;
	//world_.scale.x = 10;
	world_.UpdateMatrix();

	//ボスのｙのスケール設定

	
	//モデルセット
	modelInstance_.SetModel(resourceManager->FindModel(bossModelName));

	ValueSetting();

	//ボスの棘の初期化
	bossSpike_ = std::make_unique<BossSpike>();
	bossSpike_->Initialize(world_);


	WaitATKCount_ = 0;

	shotSpike_ = std::make_unique<ShotSpike>();
	shotSpike_->Initialize();

	

	dBossATKSpikeZone_ = std::make_unique<DangerZone>();
	dBossATKSpikeZone_->Initialize();


	//更新
	world_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(world_.worldMatrix);

	//ボスのYラインの設定
	bossYLine_ = world_.worldMatrix.m[3][1] + height_;

#pragma region HPバー

	HPBar_ = std::make_unique<Sprite>();
	HPBarFrame_ = std::make_unique<Sprite>();

	const char barname[] = "hpBar";
	auto tex = resourceManager->FindTexture(barname);
	HPBar_->SetTexture(tex);
	HPBar_->SetAnchor({ 0.0f,0.5f });

	const char barframename[] = "hpBarFrame";
	HPBarFrame_->SetTexture(resourceManager->FindTexture(barframename));
	HPBarFrame_->SetAnchor({ 0.5f,0.5f });


	HPBarTrans_.parent = &HPBarFrameTrans_;
	//仮
	HPBarFrameTrans_.translate = HPPos;

	HPBar_->SetTexcoordRect({ 0.0f,0.0f }, rects_);
	HPBarFrame_->SetTexcoordRect({ 0.0f,0.0f }, rects_);

	HPBar_->SetScale(rects_);
	HPBarFrame_->SetScale(rects_);

	HPBar_->SetDrawOrder(1);
#pragma endregion
}

void BossSecond::ValueSetting() {
	GlobalVariables* globaV = GlobalVariables::GetInstance();
	GlobalVariables::GetInstance()->CreateGroup(groupName_);

	std::string keyHP = "maxHP : 最大ヒットポイント";
	std::string keyHeight = "height : 中心点からの高さ";

	globaV->AddItem(groupName_, keyHP, maxHP_);
	globaV->AddItem(groupName_, keyHeight, height_);

	maxHP_ = globaV->GetIntvalue(groupName_, keyHP);
	HP_ = maxHP_;
	height_ = globaV->GetFloatvalue(groupName_, keyHeight);
}

void BossSecond::Update() {
#ifdef _DEBUG
	ImGui::Begin("Boss");
	ImGui::DragFloat3("pos", &world_.translate.x, 0.01f);
	ImGui::DragFloat3("scale", &world_.scale.x, 0.01f);
	ImGui::DragInt("HP", &HP_);
	ImGui::End();
#endif // _DEBUG

	dBossATKSpikeZone_->Update();

	//攻撃に関する処理
	BossATK();

	//更新
	world_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(world_.worldMatrix);
	//ボスのYラインの処理
	bossYLine_ = world_.worldMatrix.m[3][1] + height_;


#pragma region HPバー
	float hiritu = ((float)HP_ / (float)maxHP_);
	Vector2 newscale = { rects_.x * hiritu , rects_.y };
	//Vector2 newscale = scale;

	HPBar_->SetScale(newscale);

	HPBarFrameTrans_.UpdateMatrix();
	HPBarTrans_.UpdateMatrix();

	HPBarFrame_->SetPosition(HPBarFrameTrans_.translate.GetXY());
	HPBar_->SetPosition({ HPBarTrans_.worldMatrix.m[3][0] - rects_.x / 2.0f,HPBarTrans_.worldMatrix.m[3][1] });


#pragma endregion
}

bool BossSecond::IsHitBoss(const Vector3& pos, const float& wide) {

	float posminY = pos.y - wide;

	if (bossYLine_ > posminY) {
		return true;
	}


	return false;
}


int BossSecond::GetRandomNum(int wideOrmax, bool isWide) {
	unsigned int curtime = (unsigned int)time(nullptr);
	srand(curtime);
	int num;
	if (isWide) {
		num = rand() % (wideOrmax * 2 + 1) - wideOrmax;
	}
	else {
		num = rand() % wideOrmax;
	}
	return num;
}

bool BossSecond::IsHitBossATK(const Vector3& pos, const float& wide) {

	if (atkType_ == kSpikeExpATK && (atkWave_ == kWave2 || atkWave_ == kWave3)) {

		//ボスの攻撃のY座標取得
		float ATKposY = -44.0f;
		//float ATKposY = world_.translate.y + height_ * bossSpike_->GetATKScaleNum();

		//判定対象のｙ座標
		float Epos = pos.y - wide;

		//もし範囲より小さい場合
		if (ATKposY >= Epos) {
			return true;
		}
	}

	return false;

}

bool BossSecond::IsHitShotSpike(const Vector3& pos, const float wide) {
	return shotSpike_->Collision(pos, wide);
}

void BossSecond::BossATK() {


	

	//攻撃状態による更新変更
	switch (atkType_) {
	case BossSecond::kNone:
		//カウントが数え終わったら
		if (WaitATKCount_++ >= maxWaitATKCount) {
			WaitATKCount_ = 0;

			//ランダムな数字を取得
			int randomNum = GetRandomNum(3, false);
			atkType_ = (ATKType)randomNum;
			//atkType_ = kSpikeShot;
		}
		break;
	case BossSecond::kSpikeExpATK:
		SpikeAttack();
		break;

	case BossSecond::kSpikeShot:
		ShotSpikeATK();
		break;
	default:
		break;
	}
	shotSpike_->Update();

}

void BossSecond::OnCollisionExplosion(int dmg) {
	HP_ -= dmg;

	if (HP_ <= 0) {
		isDead_ = true;
	}
}

void BossSecond::OnCollisionHealing(int dmg) {
	HP_ += dmg;

	if (HP_ >= maxHP_) {
		HP_ = maxHP_;
	}
}

void BossSecond::SpikeAttack() {
	switch (atkWave_) {
	case BossSecond::kSetup:
		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;

			//警告モーション追加
			dBossATKSpikeZone_->SetCount(2, 3, dspikeZonePos, dspikeZoneScale);
		}//初期化後の処理
		else {
			float newScale = Math::Lerp(animetionT_, setUpScale.x, setUpScale.y);
			world_.scale.y = newScale;


			animetionT_ += addSetUpAnimation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				animetionT_ = 1.0f;
				newScale = Math::Lerp(animetionT_, setUpScale.x, setUpScale.y);
				world_.scale.y = newScale;


				waveInitialize_ = false;
				atkWave_ = kWave1;
			}
		}


		break;
	case BossSecond::kWave1:

		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;
		}
		else {
			float newScale = Math::Lerp(animetionT_, wave1Scale.x, wave1Scale.y);
			world_.scale.y = newScale;

			animetionT_ += addWave1Animation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				animetionT_ = 1.0f;
				newScale = Math::Lerp(animetionT_, wave1Scale.x, wave1Scale.y);
				world_.scale.y = newScale;


				waveInitialize_ = false;
				atkWave_ = kWave2;
			}
		}

		break;
	case BossSecond::kWave2:
		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;
			bossSpike_->SetStart();
		}
		else {

			bossSpike_->UpdateExtendSpike(animetionT_);

			animetionT_ += addWave2Animation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				waveInitialize_ = false;
				atkWave_ = kWave3;
			}
		}

		break;
	case BossSecond::kWave3:
		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;
		}
		else {


			animetionT_ += addWave3Animation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				waveInitialize_ = false;
				atkWave_ = Revert;
			}
		}
		break;
	case BossSecond::Revert:
		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;
		}
		else {
			bossSpike_->UpdateShrinkSpike(animetionT_);
			float newScale = Math::Lerp(animetionT_, revertScale.x, revertScale.y);
			world_.scale.y = newScale;



			animetionT_ += addWave3Animation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				animetionT_ = 1.0f;
				newScale = Math::Lerp(animetionT_, revertScale.x, revertScale.y);
				world_.scale.y = newScale;


				//いろいろ状態初期化して何もない処理の法に移動
				waveInitialize_ = false;
				atkWave_ = kSetup;
				atkType_ = kNone;
			}
		}
		break;
	default:
		break;
	}

}

void BossSecond::ShotSpikeATK() {
	switch (atkWave_) {
	case BossSecond::kSetup:
		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;

			randomNum_ = GetRandomNum(3, false);

			dBossATKSpikeZone_->SetCount(1, 2, dZoneP[randomNum_], dzoneScale);

		}
		else {
			float newScale = Math::Lerp(animetionT_, wave1Scale.x, wave1Scale.y);
			world_.scale.y = newScale;

			animetionT_ += addWave1Animation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				animetionT_ = 1.0f;
				newScale = Math::Lerp(animetionT_, wave1Scale.x, wave1Scale.y);
				world_.scale.y = newScale;


				waveInitialize_ = false;
				atkWave_ = kWave2;
			}

		}
		break;
	case BossSecond::kWave1:
		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;
		}
		else {
			float newScale = Math::Lerp(animetionT_, wave1Scale.x, wave1Scale.y);
			world_.scale.y = newScale;

			animetionT_ += addWave1Animation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				animetionT_ = 1.0f;
				newScale = Math::Lerp(animetionT_, wave1Scale.x, wave1Scale.y);
				world_.scale.y = newScale;


				waveInitialize_ = false;
				atkWave_ = kWave2;
			}
		}

		break;
	case BossSecond::kWave2:
		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;
			bossSpike_->SetStart();
		}
		else {
			Vector3 newpos = Spos[randomNum_];
			//Vector3 newpos = Spos[1];
			newpos.y = world_.translate.y;

			shotSpike_->Shot(newpos);

			animetionT_ += addWave2Animation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				waveInitialize_ = false;
				atkWave_ = Revert;
			}
		}

		break;
	case BossSecond::kWave3:
		break;
	case BossSecond::Revert:
		if (!waveInitialize_) {
			waveInitialize_ = true;
			animetionT_ = 0;
		}
		else {
			
			float newScale = Math::Lerp(animetionT_, revertScale.x, revertScale.y);
			world_.scale.y = newScale;



			animetionT_ += addWave3Animation_;

			//次のシーンに行く処理
			if (animetionT_ >= 1.0f) {
				animetionT_ = 1.0f;
				newScale = Math::Lerp(animetionT_, revertScale.x, revertScale.y);
				world_.scale.y = newScale;


				//いろいろ状態初期化して何もない処理の法に移動
				waveInitialize_ = false;
				atkWave_ = kSetup;
				atkType_ = kNone;
			}
		}
		break;
	default:
		break;
	}
}
