#pragma once

#include "Engine/Graphics/Sprite.h"
#include "Engine/Math/Transform.h"

class Heal {
public:

	Heal();
	~Heal();

	void Initalize(const Vector2& position);
	void Update();
	//void Draw();

	bool GetisAlive() { return isAlive_; }

private:
	//スプライトのインスタンス
	Sprite sprite;
	//テクスチャの位置
	Transform TexPos_;

	int AnimeFrame_ = 0;

	bool isAlive_ = true;
};