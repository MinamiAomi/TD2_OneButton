#pragma once

#include <memory>
#include <vector>

#include "Math/Transform.h"
#include "Math/Camera.h"
#include "Math/Random.h"
#include "Graphics/ToonModel.h"

class SpeedEffect {
public:
    void Initialize(const Camera* camera);
    void Update();

    void Spawn(float speed = 0.0f, uint32_t numParticles = 30);

private:
   struct Particle {
        Particle(const Vector3& position, const Vector3& scale, std::shared_ptr<ToonModel>& model);
        
        ToonModelInstance model_;
        Transform transform_;
    };

    const Camera* camera_;
    std::shared_ptr<ToonModel> model_;
    std::vector<std::unique_ptr<Particle>> particles_;
    Random::RandomNumberGenerator randomNumberGenerator_;
    float speed_;
    float deadHeight_;
};