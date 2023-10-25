#include "TitlePlayer.h"

#include "Graphics/ResourceManager.h"

const char* const TitlePlayer::Part::kModelNames[] = {
    "PlayerBody",
    "PlayerHead",
    "PlayerLArm",
    "PlayerRArm",
    "PlayerLFoot",
    "PlayerRFoot",
};

void TitlePlayer::Initialize() {
    baseTransform_.translate = Vector3::zero;
    baseTransform_.rotate = Quaternion::MakeForYAxis(Math::Pi);
    baseTransform_.scale = Vector3::one * 2.0f;

    baseTransform_.UpdateMatrix();
    InitializeParts();
}

void TitlePlayer::Update() {

    baseTransform_.UpdateMatrix();
    UpdateParts();
}

void TitlePlayer::InitializeParts() {
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    for (size_t i = 0; i < static_cast<size_t>(Part::kNumParts); ++i) {
        parts_[i] = std::make_unique<Part>();
        parts_[i]->model.SetModel(resourceManager->FindModel(Part::kModelNames[i]));
        parts_[i]->transform.scale = Vector3::one;
    }

    auto& body = parts_[static_cast<size_t>(Part::kBody)]->transform;
    auto& head = parts_[static_cast<size_t>(Part::kHead)]->transform;
    auto& lArm = parts_[static_cast<size_t>(Part::kLArm)]->transform;
    auto& rArm = parts_[static_cast<size_t>(Part::kRArm)]->transform;
    auto& lFoot = parts_[static_cast<size_t>(Part::kLFoot)]->transform;
    auto& rFoot = parts_[static_cast<size_t>(Part::kRFoot)]->transform;

    body.parent = &baseTransform_;
    head.parent = &baseTransform_;
    lArm.parent = &baseTransform_;
    rArm.parent = &baseTransform_;
    lFoot.parent = &baseTransform_;
    rFoot.parent = &baseTransform_;

    body.translate = { 0.0f, -0.75f, -1.0f };
    body.rotate = Quaternion::identity;

    head.translate = { 0.0f, 0.5f, 0.4f };
    head.rotate = Quaternion::identity;

    lArm.translate = { -1.7f, 0.0f, 0.0f };
    lArm.rotate = Quaternion::identity;

    rArm.translate = { 1.7f, 0.0f, 0.0f };
    rArm.rotate = Quaternion::identity;

    lFoot.translate = { -0.8f, -1.5f, -2.0f };
    lFoot.rotate = Quaternion::identity;

    rFoot.translate = { 0.8f, -1.5f, -2.0f };
    rFoot.rotate = Quaternion::identity;

    UpdateParts();
}

void TitlePlayer::UpdateParts() {
    for (size_t i = 0; i < static_cast<size_t>(Part::kNumParts); ++i) {
        parts_[i]->transform.UpdateMatrix();
        parts_[i]->model.SetWorldMatrix(parts_[i]->transform.worldMatrix);
    }
}
