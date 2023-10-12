#pragma once
#include"Engine/Scene/BaseScene.h"

#include<memory>

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

	Camera camera_;
	

	Map* map = nullptr;

	std::list<Spike*> spikes;
};