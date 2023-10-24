#include "LeserDust.h"
#include "Graphics/ResourceManager.h"

LeserDust::LeserDust(){}
LeserDust::~LeserDust(){}

void LeserDust::Initalize(const Vector2& position)
{
	world_.translate.x = position.x;
	world_.translate.y = position.y;
	world_.UpdateMatrix();
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char ModelName[] = "Dust";
	model_ = std::make_unique<ToonModelInstance>();
	model_->SetModel(resourceManager->FindModel(ModelName));
	model_->SetWorldMatrix(world_.worldMatrix);
	model_->SetPass(ToonModelInstance::Pass::Translucent);
	model_->SetUseOutline(false);
	model_->SetIsLighting(false);
	AnimeFrame_ = 0;
}

void LeserDust::Update()
{
	//AnimeFrame_ += 1;
	world_.UpdateMatrix();
	model_->SetWorldMatrix(world_.worldMatrix);
	if (AnimeFrame_ >= 60.0f) { 
		isAlive_ = false;
	}
#ifdef _DEBUG
	ImGui::Begin("Dust");
	ImGui::SliderFloat2("Pos", &world_.translate.x, -50.0f, 50.0f);
	ImGui::End();
#endif
}

//void Dust::Draw(){}
