#include "SpikeDust.h"
#include "Graphics/ResourceManager.h"

SpikeDust::SpikeDust() {}
SpikeDust::~SpikeDust() {}

void SpikeDust::Initalize(const Vector2& position)
{
	//シード値のrndか
	std::mt19937 mt(rnd());

	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char ModelName[] = "SpikeDust";
	//5個のモデルを同じように初期化
	for (int model_i = 0; model_i < kMaxmodel; model_i++) {
		if (model_i <= 2) {
			//移動量をランダムにする
			model_move[model_i].x = std::uniform_real_distribution<float>(0,0.05f)(rnd);
			model_move[model_i].x *= -1;
			model_move[model_i].y = std::uniform_real_distribution<float>(0,0.05f)(rnd);
			//ポジションに移動させる
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
		else if (model_i == 3) {
			//移動量をランダムにする
			model_move[model_i].x = std::uniform_real_distribution<float>(0, 0.5f)(rnd);
			//ポジションに移動させる
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
		else if (model_i >= 4) {
			//移動量をランダムにする
			model_move[model_i].x = std::uniform_real_distribution<float>(0, 0.05f)(rnd);
			model_move[model_i].y = std::uniform_real_distribution<float>(0, 0.05f)(rnd);
			//ポジションに移動させる
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
		
	}

	AnimeFrame_ = 0;
}

void SpikeDust::Update()
{
	//モデルをそれぞれ移動させる
	for (int model_i = 0; model_i < kMaxmodel; model_i++) {
		model_world[model_i].translate.x += model_move[model_i].x;

		model_move[model_i].y -= gravity;
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

void SpikeDust::InitalizeRandom()
{
	

}