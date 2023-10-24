#include "SpeedEffect.h"

#include <cassert>

#include "Graphics/ResourceManager.h"

void SpeedEffect::Initialize(const Camera* camera) {
    assert(camera);

    ResourceManager* resourceManager = ResourceManager::GetInstance();

    camera_ = camera;

    model_ = resourceManager->FindModel("Pole");
}

void SpeedEffect::Update() {
    speed_ += 0.05f;

    for (auto& particle : particles_) {
        if (particle) {
            particle->transform_.translate.y -= speed_;
            particle->transform_.UpdateMatrix();
            particle->model_.SetWorldMatrix(particle->transform_.worldMatrix);
        }
    }
    std::erase_if(particles_, [&](auto& particle) {
        return particle->transform_.translate.y < deadHeight_;
        });
}

void SpeedEffect::Spawn(float speed, uint32_t numParticles) {
    Matrix4x4 viewProjection = camera_->GetViewProjectionMatrix();
    Matrix4x4 inverse = viewProjection.Inverse();

    Vector3 origin = viewProjection.ApplyTransformWDivide(Vector3::zero);

    Vector3 spawnLeftUp = inverse.ApplyTransformWDivide({ -1.0f, 10.0f, origin.z });
    Vector3 spawnRightBottom = inverse.ApplyTransformWDivide({ 1.0f,1.2f, origin.z });
    deadHeight_ = inverse.ApplyTransformWDivide({ 0.0f, -2.0f, origin.z }).y;

    particles_.resize(numParticles);
    for (auto& particle : particles_) {
        Vector3 position, scale;
        position.x = randomNumberGenerator_.NextFloatRange(spawnLeftUp.x, spawnRightBottom.x);
        position.y = randomNumberGenerator_.NextFloatRange(spawnRightBottom.y, spawnLeftUp.y);
        position.z = spawnLeftUp.z;
        scale.x = randomNumberGenerator_.NextFloatRange(0.11f, 0.2f);
        scale.y = randomNumberGenerator_.NextFloatRange(5.0f, 12.0f);
        scale.z = 1.0f;
        particle = std::make_unique<Particle>(position, scale, model_);
    }

    speed_ = speed;
}

SpeedEffect::Particle::Particle(const Vector3& position, const Vector3& scale, std::shared_ptr<ToonModel>& model) {
    transform_.translate = position;
    transform_.scale = scale;
    transform_.UpdateMatrix();
    model_.SetModel(model);
    model_.SetPass(ToonModelInstance::Pass::Translucent);
    model_.SetAlpha(0.2f);
}
