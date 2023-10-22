#include"Boss.h"

#include "Graphics/ResourceManager.h"
#include"GlobalVariables.h"

void Boss::Initalize(const Vector3& position)
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char bossModelName[] = "Boss";

	//座標設定
	world_.translate = position;
	//world_.scale.x = 10;
	world_.UpdateMatrix();

	//ボスのｙのスケール設定
	height_ = world_.scale.y;

	//ボスのYラインの設定
	bossYLine_ = world_.worldMatrix.m[3][1] + height_;

	//モデルセット
	modelInstance_.SetModel(resourceManager->FindModel(bossModelName));

	ValueSetting();

}

void Boss::ValueSetting() {
	GlobalVariables* globaV = GlobalVariables::GetInstance();
	GlobalVariables::GetInstance()->CreateGroup(groupName_);

	std::string keyHP = "maxHP : 最大ヒットポイント";
	std::string keyHeight = "height : 中心点からの高さ";

	globaV->AddItem(groupName_, keyHP, maxHP_);
	globaV->AddItem(groupName_, keyHeight, height_);

	maxHP_ = globaV->GetIntvalue(groupName_, keyHP);
	height_ = globaV->GetFloatvalue(groupName_, keyHeight);
}

void Boss::Update()
{
	//更新
	world_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(world_.worldMatrix);
	//ボスのYラインの処理
	bossYLine_ = world_.worldMatrix.m[3][1] + height_;

}

bool Boss::IsHitBoss(const Vector3& pos, const float& wide)
{
	
	float posminY = pos.y - wide;

	if (bossYLine_ > posminY) {
		return true;
	}

	
	return false;
}

void Boss::OnCollisionExplosion(int dmg) {
	HP_ -= dmg;

	if (HP_ <= 0) {
		isDead_ = true;
	}
}

void Boss::OnCollisionHealing(int dmg)
{
	HP_ += dmg;

	if (HP_ >= maxHP_) {
		HP_ = maxHP_;
	}
}
