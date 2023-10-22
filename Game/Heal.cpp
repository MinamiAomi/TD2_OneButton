#include "Heal.h"
#include "Graphics/ResourceManager.h"

Heal::Heal() {}
Heal::~Heal() {}

void Heal::Initalize()
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char bossModelName[] = "Heal";
	sprite.SetTexture(resourceManager->FindTexture(bossModelName));
	sprite.SetPosition(TexPos_.translate.GetXY());
	sprite.SetRotate(0.0f);
	sprite.SetScale({ 32.0f,32.0f });
	sprite.SetTexcoordRect({ 0.0f,0.0f }, { 32,32 });
}

void Heal::Update(){
}

//void Heal::Draw(){}

