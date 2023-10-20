#pragma once
#include<Engine/Scene/BaseScene.h>
#include<Engine/Scene/SceneManager.h>

class StageSerect : public BaseScene {

public:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;


private:
	SceneManager* sceneManager_;
};