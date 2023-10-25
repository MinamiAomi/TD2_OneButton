#include "LeserDust.h"
#include "Graphics/ResourceManager.h"

LeserDust::LeserDust(){}
LeserDust::~LeserDust(){}

void LeserDust::Initalize(const Vector2& position)
{
	
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char ModelName[] = "Dust";
	//5個のモデルを同じように初期化
	for (int model_i = 0; model_i < kMaxmodel;model_i++) {
		model_world[model_i].translate.x = position.x;
		model_world[model_i].translate.y = position.y;
		model_world[model_i].UpdateMatrix();
		model_[model_i] = std::make_unique<ToonModelInstance>();
		model_[model_i]->SetModel(resourceManager->FindModel(ModelName));
		model_[model_i]->SetWorldMatrix(model_world[model_i].worldMatrix);
		model_[model_i]->SetPass(ToonModelInstance::Pass::Translucent);
		model_[model_i]->SetUseOutline(false);
		model_[model_i]->SetIsLighting(false);
	}
	
	AnimeFrame_ = 0;
}

void LeserDust::Update()
{
	//モデルをそれぞれ移動させる
	for (int model_i = 0; model_i < kMaxmodel; model_i++) {
		model_world[model_i].translate.x += model_move[model_i].x;
		model_world[model_i].translate.y += model_move[model_i].y;
		
		model_world[model_i].UpdateMatrix();
		model_[model_i]->SetWorldMatrix(model_world[model_i].worldMatrix);
	}
	//アニメフレームを加算
	AnimeFrame_ += 1;
	//アニメフレームが規定値を超えたらフラグをオフにする
	if (AnimeFrame_ >= 45.0f) {
		isAlive_ = false;
	}
}

//void Dust::Draw(){}
