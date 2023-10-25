#include "Background.h"

#include <cassert>

#include "Graphics/ResourceManager.h"

void Background::Initialize(const Camera* camera) {
    assert(camera);

    base_ = std::make_unique<ToonModelInstance>();
    tree1_ = std::make_unique<ToonModelInstance>();
    tree2_ = std::make_unique<ToonModelInstance>();
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    base_->SetModel(resourceManager->FindModel("BaseBackGround"));
    base_->SetUseOutline(false);
    base_->SetIsLighting(false);
    tree1_->SetModel(resourceManager->FindModel("BackTree1"));
    tree1_->SetUseOutline(false);
    tree1_->SetIsLighting(false);
    tree2_->SetModel(resourceManager->FindModel("BackTree2"));
    tree2_->SetUseOutline(false);
    tree2_->SetIsLighting(false);

    scroll_ = 0.0f;

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
    {
        float offset = std::fmod(scroll_, 2.0f);
        Vector3 translate = { 0.0f, Math::Lerp(-0.5f + offset, quad[0].y, quad[1].y), quad[0].z };
        Quaternion rotate = Quaternion::MakeForYAxis(Math::Pi);
        Vector3 scale = { quad[2].x - quad[0].x, quad[0].y - quad[1].y, 1.0f };

        tree1_->SetWorldMatrix(Matrix4x4::MakeAffineTransform(scale, rotate, translate));
    }

    {
        float offset = std::fmod(scroll_ + 1.0f, 2.0f);
        Vector3 translate = { 0.0f, Math::Lerp(-0.5f + offset, quad[0].y, quad[1].y), quad[0].z };
        Vector3 scale = { quad[2].x - quad[0].x, quad[0].y - quad[1].y, 1.0f };

        Quaternion rotate = Quaternion::MakeForYAxis(Math::Pi);
        tree2_->SetWorldMatrix(Matrix4x4::MakeAffineTransform(scale, rotate, translate));
    }
    {
        Vector3 translate = { 0.0f, Math::Lerp(0.5f, quad[0].y, quad[1].y), quad[0].z };
        Quaternion rotate = Quaternion::MakeForYAxis(Math::Pi)/*Quaternion::identity*/;
        Vector3 scale = { quad[2].x - quad[0].x, quad[0].y - quad[1].y, 1.0f };
        base_->SetWorldMatrix(Matrix4x4::MakeAffineTransform(scale, rotate, translate));
    }
}

void Background::Scroll(float deltaScroll) {
    scroll_ += deltaScroll;
}
