#include"dangerZone.h"
#include "Graphics/ResourceManager.h"
#include"Externals/ImGui/imgui.h"
#include"Math/MathUtils.h"

void DangerZone::Initialize(Vector3 pos,Vector3 scale) {

	ResourceManager* resourceManager = ResourceManager::GetInstance();

	const char dangerModelName[] = "danger";
	model.SetModel(resourceManager->FindModel(dangerModelName));

	model.SetPass(ToonModelInstance::Pass::Translucent);
	model.SetIsActive(false);

	world.translate = pos;
	world.scale = scale;

	Vector3 R = { 0.0f,180.0f,0.0f };
	world.rotate = Quaternion::MakeFromEulerAngle(R * Math::ToRadian);
}

void DangerZone::SetCount(int second,int loopNum) {
	//秒数ｘフレーム
	int count = second * 60;

	countOneF = (count / loopNum) / 2.0f;

	loopGetter = (float)loopNum;
	loopCounter = 0;

	isAdd_ = true;
	isActive_ = true;

	t_ = 0;
}

void DangerZone::Update() {

#ifdef _DEBUG
	
	ImGui::Begin("DZone");
	ImGui::DragFloat3("pos", &world.translate.x, 0.01f);
	ImGui::DragFloat3("scale", &world.scale.x, 0.01f);
	ImGui::DragFloat("alpha", &Alpha, 0.01f);
	
	ImGui::End();
	
#endif // _DEBUG
	if (isActive_) {


		LoopAnimation();

		
		model.SetAlpha(Alpha);
		world.UpdateMatrix();
		model.SetWorldMatrix(world.worldMatrix);
	}
	else {
		
	}
	model.SetIsActive(isActive_);
}

void DangerZone::LoopAnimation() {

	//カウンターと同じ数字になるまで処理
	if (loopGetter != loopCounter) {
		if (isAdd_) {

			t_ += 1.0f / countOneF;

			Alpha = Math::Lerp(t_, alphaMin, alphaMax);

			if (t_ >= 1.0f) {
				isAdd_ = false;
				t_ = 0;
			}
		}
		else {
			t_ += 1.0f / countOneF;

			Alpha = Math::Lerp(t_, alphaMax, alphaMin);

			if (t_ >= 1.0f) {
				isAdd_ = true;
				t_ = 0;
				loopCounter++;
			}
		}
	}
	else {
		model.SetIsActive(false);
		isActive_=false;
	}
}
