#include "Dust.h"
#include "Graphics/ResourceManager.h"

Dust::Dust(){}
Dust::~Dust(){}

void Dust::Initalize(const Vector2& position)
{
	world_.translate.x = position.x;
	world_.translate.y = position.y;
	world_.UpdateMatrix();
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char ModelName[] = "Heal";
	model_->SetModel(resourceManager->FindModel(ModelName));
	model_->SetWorldMatrix(world_.worldMatrix);
	model_->SetPass(ToonModelInstance::Pass::Translucent);
	model_->SetUseOutline(false);
	model_->SetIsLighting(false);
	AnimeFrame_ = 0;
}

void Dust::Update()
{
	AnimeFrame_ += 1;
	if (AnimeFrame_ >= 60.0f) {
		isAlive_ = false;
	}
}

//void Dust::Draw(){}
