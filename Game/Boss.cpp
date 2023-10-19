#include"Boss.h"

void Boss::Initalize(const Vector3& position, std::shared_ptr<ToonModel> toonModel)
{
	//座標設定
	world_.translate = position;
	//world_.scale.x = 10;
	world_.UpdateMatrix();

	//ボスのｙのスケール設定
	height_ = world_.scale.y;

	//ボスのYラインの設定
	bossYLine_ = world_.worldMatrix.m[3][1] + height_;

	//モデルセット
	modelInstance_.SetModel(toonModel);

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
	//ボス座標取得
	Vector3 bosspos = GetMatWT();

	//x座標を合わせる
	bosspos.x = pos.x;

	//差分計算
	Vector3 diff = pos - bosspos;

	//距離計算
	float Hlength = sqrtf(diff.x * diff.x + diff.y * diff.y);

	//当たっていたら処理
	if (Hlength <= wide + height_) {
		return true;
	}
	
	return false;
}

void Boss::OnCollisionHealing()
{
	HP_++;
}
