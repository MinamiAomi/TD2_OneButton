#include "TitlePlayer.h"

#include "Graphics/ResourceManager.h"

const char* TitlePlayer::Part::kModelNames[] = {
    "PlayerHead",
    "PlayerBody",
    "PlayerLArm",
    "PlayerRArm",
    "PlayerLFoot",
    "PlayerRFoot",
};

void TitlePlayer::Initialize() {
    baseTransform_.translate = Vector3::zero;
    baseTransform_.rotate = Quaternion::identity;
    baseTransform_.scale = Vector3::one;

    InitializeParts();
}

void TitlePlayer::Update() {

}

void TitlePlayer::InitializeParts() {
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    for (size_t i = 0; i < static_cast<size_t>(Part::kNumParts); ++i) {
        parts_[i] = std::make_unique<Part>();
        parts_[i]->model.SetModel(resourceManager->FindModel(Part::kModelNames[i]));
    }

    auto& head = parts_[static_cast<size_t>(Part::kHead)]->transform;
    head.translate = { 0.0f,0.0f,0.0f };
    head.rotate = Quaternion::identity;

}

void TitlePlayer::UpdateParts() {

}
