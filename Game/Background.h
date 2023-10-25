#pragma once

#include <memory>

#include "Math/Transform.h"
#include "Math/Camera.h"
#include "Graphics/ToonModel.h"

class Background {
public:
    void Initialize(const Camera* camera);
    void Update();

    void Scroll(float deltaScroll);

private:
    std::unique_ptr<ToonModelInstance> base_;
    std::unique_ptr<ToonModelInstance> tree1_;
    std::unique_ptr<ToonModelInstance> tree2_;
    float scroll_;
    const Camera* camera_;
};