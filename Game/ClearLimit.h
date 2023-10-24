#pragma once

#include "Graphics/Sprite.h"

class ClearLimit {
public:
	void Initialize();

	void Update(int limit);

private:

	struct Sprites {
		std::unique_ptr<Sprite> spirte;
		Vector2 position;

	};

	//桁数
	static const int digitNum = 3;

	//三桁分作成
	Sprites numSprite_[digitNum];

	Sprites meterSprite_;

	//中心座標
	Vector2 center_ = { 270.0f,365.0f };

	//各桁の数字
	int num_[3] = { 0,0,0 };


	//一つの文字のサイズ
	const float texSize = 32.0f;

	const float texScale_ = 192;

	Vector2 mPos_ = { -9.0f,-36.0f };
	Vector2 mScale_ = { 80.0f,80.0f };
};
