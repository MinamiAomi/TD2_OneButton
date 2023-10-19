#include"Boss.h"

#include "Graphics/ResourceManager.h"

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
	Vector3 bosspos = GetMatWT();
	//x座標を合わせる
	bosspos.x = pos.x;

	Vector3 diff = pos - bosspos;

	float Hlength = sqrtf(diff.x * diff.x + diff.y * diff.y);

	if (Hlength <= wide + height_) {
		return true;
	}
	
	return false;
}

void Boss::OnCollisionHealing()
{
	HP_++;
}
