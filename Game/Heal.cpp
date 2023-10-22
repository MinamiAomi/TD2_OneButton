#include "Heal.h"
#include "Graphics/ResourceManager.h"

Heal::Heal() {}
Heal::~Heal() {}

void Heal::Initalize(const Vector2& position)
{
	TexPos_.translate.x = position.x;
	TexPos_.translate.y = position.y;
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char bossModelName[] = "Heal";
	sprite.SetTexture(resourceManager->FindTexture(bossModelName));
	sprite.SetPosition(TexPos_.translate.GetXY());
	sprite.SetScale({ 32.0f,32.0f });
	sprite.SetTexcoordRect({ 0.0f,0.0f }, { 32,32 });
}

void Heal::Update(){

}


//void Heal::Draw(){}

