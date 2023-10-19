#include "Leser.h"

#include "Graphics/ResourceManager.h"

Leser::Leser() {}
Leser::~Leser() {}


float Esing(const float& start, const float& end, const float& t) {
	return start * (1.0f - t) + end * t;
}



void Leser::Initialize(const Vector3& playerPos, const Vector3& bossPos)
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char laserModelName[] = "Laser";
	const char explosionModelName[] = "Explosion";

#pragma region ビーム関連
	//モデルセット
	leserModelInstance_.SetModel(resourceManager->FindModel(laserModelName));
	//中心座標取得
	Vector3 velo = (playerPos - bossPos) / 2;
	//中心点
	Vector3 pos = bossPos + velo;

	//ｙScale取得
	float yScale = sqrtf(velo.x * velo.x + velo.y * velo.y);

	//変化するwide初期化
	variableWide_ = leserWide_;

	//Transform再設定
	leserTransform_.translate = pos;
	leserTransform_.scale = { leserWide_,yScale,leserWide_ };
#pragma endregion

#pragma region 爆発関連
	//モデルセット
	expModelInstance_.SetModel(resourceManager->FindModel(explosionModelName));

	//爆発の情報取得と設定
	explosionpos_.translate = bossPos;
	explosionRadius_ = 0;
	explosionpos_.scale = { explosionRadius_,explosionRadius_ ,explosionRadius_ };

#pragma endregion




	//描画＆処理フラグＯＮ
	IsAlive_ = true;
}

void Leser::Update() {
	
	//アクティブ中に処理
	if (IsAlive_) {
		
#pragma region ビーム関連
		leserTransform_.rotate.y += 1.0f / 30.0f;

		//処理
		leserTransform_.scale.x = variableWide_;
		leserTransform_.scale.z = variableWide_;

		
		//行列と描画用処理
		leserTransform_.UpdateMatrix();
		leserModelInstance_.SetWorldMatrix(leserTransform_.worldMatrix);
#pragma endregion

#pragma region 爆破関連
		//更新処理
		explosionpos_.scale = { explosionRadius_,explosionRadius_ ,explosionRadius_ };

		explosionpos_.UpdateMatrix();
		expModelInstance_.SetWorldMatrix(explosionpos_.worldMatrix);
#pragma endregion

		//T加算
		t_ += addT_;

		//各種数値変更
		variableWide_ = Esing(leserWide_, endLeserWide_, t_);

		explosionRadius_ = Esing(minExplosionRadius_, maxExplosionRadius_, t_);

		//t=1で終了
		if (t_>=1.0f) {
			IsAlive_ = false;
		}	
	}
}


void Leser::OnCollision() {
	IsAlive_ = false;
}