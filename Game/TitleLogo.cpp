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

    animationCycle_ = 450;
    animationParameter_ = 0;

    center_ = { 270.0f, 500 };

    for (uint32_t i = 0; i < kNumLetters; ++i) {
        letters_[i].spirte = std::make_unique<Sprite>();
        float textureBase = i * textureLetterSize;
        letters_[i].spirte->SetTexture(texture);
        letters_[i].spirte->SetTexcoordRect({ textureBase, 0.0f }, { textureLetterSize, textureLetterSize });
        Vector2 offset = { letterDistanceTable[i], 0.0f };
        letters_[i].spirte->SetPosition(center_ + offset);
        letters_[i].spirte->SetScale({ 50.0f, 50.0f });

        uint32_t animationParameterByLetter = animationCycle_ / kNumLetters;
        letters_[i].animationOffset = i * animationParameterByLetter / 2;
        letters_[i].animationLength = animationParameterByLetter;
    }

}

void TitleLogo::Update() {

    static bool isAnimation = false;


    if (isAnimation) {
        ++animationParameter_;
        if (animationParameter_ > animationCycle_) {
            animationParameter_ = 0;
        }

        auto Ease = [](float x) {
            return x;
            };
      

        for (uint32_t letterIndex = 0; letterIndex < kNumLetters; ++letterIndex) {
            auto& letter = letters_[letterIndex];

            // 範囲外のためアニメーションしない
            if (animationParameter_ < letter.animationOffset ||
                animationParameter_ > letter.animationOffset + letter.animationLength) {
                continue;
            }

            float t = float(animationParameter_ - letter.animationOffset) / letter.animationLength;
            t = Ease(t);
            Vector2 animationOffset = { letterDistanceTable[letterIndex], std::sin(t * Math::Pi) * 100.0f };

            letter.spirte->SetPosition(center_ + animationOffset);
            float sT = (t < 0.5f ? t : 1.0f - t) * 2.0f;
            Vector2 scale = Vector2::Lerp(sT, { 50.0f, 50.0f }, { 85.0f, 85.0f });
            letter.spirte->SetScale(scale);
        }
    }

#ifdef _DEBUG
    ImGui::Begin("TitleLogo");
    int p = int(animationParameter_);
    ImGui::DragInt("AnimationParameter", &p, 1, 0, animationCycle_);
    animationParameter_ = uint32_t(p);
    ImGui::Checkbox("IsAnimation", &isAnimation);
    ImGui::End();

#endif // _DEBUG
}
