#pragma once
#include "Engine/Scene/BaseScene.h"

#include <memory>

#include "Engine/Graphics/ToonModel.h"
#include "Math/Camera.h"
#include "Math/Transform.h"


class TitleScene : public BaseScene {
public:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private:
	Camera camera_;
};