#pragma once

#include "Graphics/Sprite.h"

class TitleLogo {
public:
    void Initialize();
    void Update();

private:
    // タイトル文字数
    static const uint32_t kNumLetters = 12;

    struct Letter {
        std::unique_ptr<Sprite> spirte;
        Vector2 position;
    };

    static const char* letterTable[kNumLetters];
    static float letterDistanceTable[kNumLetters];

    Letter letters_[kNumLetters];
    Vector2 center_;
};