#include"shotSpike.h"
#include "Graphics/ResourceManager.h"


void ShotSpike::Initialize() {
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	const char* modelName = "BossSpike";
	model_.SetModel(resourceManager->FindModel(modelName));

	world_.scale.x = 1.0f / 3.0f;
	world_.translate.z -= 1;
	isActive_ = false;

}

void ShotSpike::Shot(Vector3 pos) {
	isActive_ = true;
	world_.translate = pos;
	deadCount_ = 30;
}

void ShotSpike::Update() {
	if (isActive_) {
		world_.translate += velo;
		if (deadCount_-- <= 0) {
			isActive_ = false;
		}
		model_.SetIsActive(isActive_);
		world_.UpdateMatrix();
		model_.SetWorldMatrix(world_.worldMatrix);
	}
}
