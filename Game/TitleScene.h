#pragma once
#include "Engine/Scene/BaseScene.h"

#include <memory>

#include "Engine/Graphics/ToonModel.h"
#include "Math/Camera.h"
#include "Math/Transform.h"

#include "TitleLogo.h"

class TitleScene : public BaseScene {
public:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private:
	Camera camera_;

	std::unique_ptr<TitleLogo> titleLogo_;
	
};