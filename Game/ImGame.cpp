#include"ImGame.h"

#include "Engine/Graphics/RenderManager.h"
#include"Engine/Graphics/ModelLoader.h"
#include "Engine/Scene/SceneManager.h"


#include "Math/Transform.h"

void InGame::OnInitialize()
{

	RenderManager::GetInstance()->SetCamera(camera_);

	std::shared_ptr<ToonModel> toonModel_ = std::make_shared<ToonModel>();
	toonModel_->Create(ModelData::LoadObjFile("Resources/Model/sphere.obj"));
	
	ToonModelInstance modelInstance_;
	modelInstance_.SetModel(toonModel_);

	
	
	//std::vector<std::shared_ptr<ToonModel>> modelMap = { toonModel_ };


	map = new Map();
	map->Initialize();

	std::vector<Transform> spikeWorld = map->GetSpikeWorld();

	int spileNum = (int)spikeWorld.size();

	for (int num = 0; num < spileNum; num++) {
		Spike* spike_ = new Spike();
		spike_->Initialize(spikeWorld[num], toonModel_, 0);
		spikes.push_back(spike_);
	}
}

void InGame::OnUpdate()
{

	map->Update();

	for (Spike* spike : spikes) {
		spike->Update();
	}

	
	camera_.UpdateMatrices();

	GetAllCollisions();
	CheckDead();

}




void InGame::GetAllCollisions() {
	//スパイクのWorld
	for (Spike* spike : spikes) {
		spike->GetmatWtranstate();

	}
}

void InGame::CheckDead() {
	spikes.remove_if([](Spike* spike) {
		if (spike->IsDead()) {
			delete spike;
			return true;
		}
		return false;
		});
}


void InGame::OnFinalize()
{
}