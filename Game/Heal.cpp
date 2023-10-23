#include "Heal.h"
#include "Graphics/ResourceManager.h"

Heal::Heal() {}
Heal::~Heal() {}

void Heal::Initalize(const Vector2& position)
{
	TexPos_.translate.x = position.x;
	TexPos_.translate.y = -59;
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	const char bossModelName[] = "Heal";
	sprite.SetTexture(resourceManager->FindTexture(bossModelName));
	sprite.SetPosition(TexPos_.translate.GetXY());
	sprite.SetScale({ 32.0f,32.0f });
	sprite.SetTexcoordRect({ 0.0f,0.0f }, { 32,32 });
	AnimeFrame_ = 0;
}

void Heal::Update(){
	TexPos_.translate.y += 1.0f;
	AnimeFrame_ += 1;
	if (AnimeFrame_ >= 20.0f) {
		isAlive_ = false;
	}

}


//void Heal::Draw(){}

