#pragma once

#include "Graphics/Sprite.h"

class Limit {
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
	

	//中心座標
	const Vector2 center_= { 270.0f,500.0f};
	
	//各桁の数字
	int num_[3] = { 0,0,0 };


	//一つの文字のサイズ
	const float texSize = 32.0f;

	const float texScale_=192;

};