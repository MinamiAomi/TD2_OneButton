#include"BossSpike.h"
#include "Graphics/ResourceManager.h"

void BossSpike::Initialize(Transform world) {

	ResourceManager* resourceManager = ResourceManager::GetInstance();

	const char* modelName = "BossSpike";
	model_.SetModel(resourceManager->FindModel(modelName));


	world_ = world;
	world_.translate.z += 1.0f;
	world_.scale.y = 0;
}

void BossSpike::SetStart() {
	world_.scale.y = 0;
}

void BossSpike::UpdateExtendSpike(const float& t) {
	float newScale = Math::Lerp(t, extendScale.x, extendScale.y);
	world_.scale.y = newScale;

	world_.UpdateMatrix();
	model_.SetWorldMatrix(world_.worldMatrix);
}

void BossSpike::UpdateShrinkSpike(const float& t) {
	float newScale = Math::Lerp(t, shrinkScale.x, shrinkScale.y);
	world_.scale.y = newScale;

	world_.UpdateMatrix();
	model_.SetWorldMatrix(world_.worldMatrix);
}

