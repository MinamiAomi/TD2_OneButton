#include "TitleLogo.h"

#include "Graphics/ImGuiManager.h"
#include "Graphics/ResourceManager.h"

const char* TitleLogo::letterTable[TitleLogo::kNumLetters] = {
    "M", "E", "C", "H", "A", "M", "O", "M", "O", "N", "G", "A"

};

float TitleLogo::letterDistanceTable[TitleLogo::kNumLetters] = {
    /*M*/-169,
    /*E*/-134,
    /*C*/-107,
    /*H*/ -80,
    /*A*/ -53,
    /*M*/ -20,
    /*O*/  15,
    /*M*/  49,
    /*O*/  83,
    /*N*/ 113,
    /*G*/ 144,
    /*A*/ 171,
};

void TitleLogo::Initialize() {

    auto resourceManager = ResourceManager::GetInstance();

    float textureLetterSize = 64.0f;
    const char textureName[] = "TitleLogo";
    auto texture = resourceManager->FindTexture(textureName);

    center_ = { 270.0f, 500 };

    for (uint32_t i = 0; i < kNumLetters; ++i) {
        letters_[i].spirte = std::make_unique<Sprite>();
        float textureBase = i * textureLetterSize;
        letters_[i].spirte->SetTexture(texture);
        letters_[i].spirte->SetTexcoordRect({ textureBase, 0.0f }, { textureLetterSize, textureLetterSize });
        Vector2 offset = { letterDistanceTable[i], 0.0f };
        letters_[i].spirte->SetPosition(center_ + offset);
        letters_[i].spirte->SetScale({ 50.0f, 50.0f });
    }

}

void TitleLogo::Update() {

#ifdef _DEBUG
    ImGui::Begin("TitleLogo");
    for (uint32_t i = 0; i < kNumLetters; ++i) {
        ImGui::DragFloat((letterTable[i] + std::to_string(i)).c_str(), &letterDistanceTable[i]);
        Vector2 offset = { letterDistanceTable[i], 0.0f };
        letters_[i].spirte->SetPosition(center_ + offset);
    }
    ImGui::End();

#endif // _DEBUG
}
