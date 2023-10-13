#pragma once
#include"Engine/Scene/BaseScene.h"
#include "Engine/Graphics/ToonModel.h"
#include "Math/Camera.h"



#include"Spike.h"
#include"map.h"
#include<list>

class InGame : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;


	
	/// <summary>
	/// すべての当たり判定処理まとめ
	/// </summary>
	void GetAllCollisions();

	/// <summary>
	/// 死亡判定
	/// </summary>
	void CheckDead();


	
private:
	std::shared_ptr<ToonModel> toonModel_;
	ToonModelInstance modelInstance_;
	Camera camera_;
	
	
	std::unique_ptr<Map>map = nullptr;

	std::list<Spike*> spikes;
};