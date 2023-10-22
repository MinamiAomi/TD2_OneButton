#pragma once

#include "Engine/Graphics/Sprite.h"
#include "Engine/Math/Transform.h"

class Heal {
public:

	Heal();
	~Heal();

	void Initalize();
	void Update();
	//void Draw();

private:
	//スプライトのインスタンス
	Sprite sprite;
	//テクスチャの位置
	Transform TexPos_;
};