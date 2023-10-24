#pragma once

#include <memory>

#include "Math/Transform.h"
#include "Math/Camera.h"
#include "Graphics/ToonModel.h"

class Background {
public:
    void Initialize(const Camera* camera);
    void Update();

private:
    std::unique_ptr<ToonModelInstance> model_;
    const Camera* camera_;
};