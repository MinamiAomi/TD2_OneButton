#include "Background.h"

#include <cassert>

#include "Graphics/ResourceManager.h"

void Background::Initialize(const Camera* camera) {
    assert(camera);

    model_ = std::make_unique<ToonModelInstance>();
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    const char modelName[] = "Background";
    model_->SetModel(resourceManager->FindModel(modelName));
    model_->SetUseOutline(false);
    model_->SetIsLighting(false);

    camera_ = camera;
}

void Background::Update() {
    assert(camera_);
    // カメラの逆行列を取得
    Matrix4x4 viewProjectionInverse = camera_->GetViewProjectionMatrix().Inverse();
    Vector3 quad[] = {
        { -1.0f,  1.0f, 0.99f }, // 左上
        { -1.0f, -1.0f, 0.99f }, // 左下
        {  1.0f,  1.0f, 0.99f }, // 右上
    };

    for (auto& v : quad) {
        v = viewProjectionInverse.ApplyTransformWDivide(v);
    }

    Vector3 translate = { Math::Lerp(0.5f, quad[2].x, quad[0].x), Math::Lerp(0.5f, quad[0].y, quad[1].y), quad[0].z };
    Quaternion rotate = Quaternion::MakeForYAxis(Math::Pi )/*Quaternion::identity*/;
    Vector3 scale = { quad[2].x - quad[0].x, quad[0].y - quad[1].y, 1.0f };
    model_->SetWorldMatrix(Matrix4x4::MakeAffineTransform(scale, rotate, translate));
}
