#include "Heal.h"
#include "Graphics/ResourceManager.h"

Heal::Heal() {}
Heal::~Heal() {}

void Heal::Initalize(const Vector2& position)
{
	model_ = std::make_unique<ToonModelInstance>();
	TexPos_.translate.x = position.x;
	TexPos_.translate.y = position.y;
	TexPos_.UpdateMatrix();
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char ModelName[] = "Heal";
	model_->SetModel(resourceManager->FindModel(ModelName));
	model_->SetWorldMatrix(TexPos_.worldMatrix);
	model_->SetPass(ToonModelInstance::Pass::Translucent);
	model_->SetUseOutline(false);
	model_->SetIsLighting(false);
	AnimeFrame_ = 0;
}

void Heal::Update(){
	TexPos_.translate.y += 0.1f;
	TexPos_.UpdateMatrix();
	model_->SetWorldMatrix(TexPos_.worldMatrix);
	AnimeFrame_ += 1;
	if (AnimeFrame_ >= 60.0f) {
		isAlive_ = false;
	}
#ifdef _DEBUG
	ImGui::Begin("Heal");
	ImGui::SliderFloat2("Pos", &TexPos_.translate.x,-50.0f,50.0f);
	ImGui::End();
#endif
}


//void Heal::Draw(){}

