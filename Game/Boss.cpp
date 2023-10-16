#include"Boss.h"

void Boss::Initalize(const Vector3& position, std::shared_ptr<ToonModel> toonModel)
{
	//座標設定
	world_.translate = position;
	world_.scale.x = 10;
	wide_ = world_.scale.y;

	//モデル描画
	modelInstance_.SetModel(toonModel);

}

void Boss::Update()
{

	world_.UpdateMatrix();
	modelInstance_.SetWorldMatrix(world_.worldMatrix);
}
