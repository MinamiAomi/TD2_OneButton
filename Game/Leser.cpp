#include "Leser.h"

Leser::Leser() {}
Leser::~Leser() {}

void Leser::Initalize(std::shared_ptr<ToonModel> model, const Vector3& playerPos, const Vector3& bossPos) {
	//モデルセット
	modelInstance_.SetModel(model);

	//中心座標取得
	Vector3 velo = (playerPos - bossPos) / 2;
	//中心点
	Vector3 pos = bossPos + velo;

	//ｙScale取得
	float yScale = sqrtf(velo.x * velo.x + velo.y * velo.y);

	//変化するwide初期化
	variableWide_ = leserWide_;

	//Transform再設定
	worldTransform_.translate = pos;
	worldTransform_.scale = { leserWide_,yScale,leserWide_ };

	//描画＆処理フラグＯＮ
	IsAlive = true;
}



void Leser::Update() {
	
	//アクティブ中に処理
	if (IsAlive) {
		//AliveCount--;
		//if (AliveCount <= 0) {
		//	IsAlive = false;
			
		//	return;
		//}

		worldTransform_.rotate.y  += 1.0f / 30.0f;

		//処理
		worldTransform_.scale.x = variableWide_;
		worldTransform_.scale.z = variableWide_;

		//半径量減少
		variableWide_ -= SubWide_;

		//0以下で終了
		if (variableWide_ <= 0.0f) {
			IsAlive = false;
		}


		//行列と描画用処理
		worldTransform_.UpdateMatrix();
		modelInstance_.SetWorldMatrix(worldTransform_.worldMatrix);
	}


	//描画するか否か
	//modelInstance_.SetIsActive(IsAlive);
}


void Leser::OnCollision() {
	IsAlive = false;

}