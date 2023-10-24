#include "Graphics/ResourceManager.h"
#include"Externals/ImGui/imgui.h"
#include"GlobalVariables.h"
#include"Clear.h"
#include"TitleScene.h"
#include"ImGame.h"

void Clear::OnInitialize() {
    input_ = Input::GetInstance();

    ResourceManager* resourceManager = ResourceManager::GetInstance();
    //仮画像
    const char textureName[] = "resultImage1";

    clearTrans_.translate = { 270.0f,360.0f,0.0f };

    Vector2 texScale = { 540.0f,720.0f };

    clearTex_.SetTexture(resourceManager->FindTexture(textureName));
    clearTex_.SetPosition(clearTrans_.translate.GetXY());
    clearTex_.SetAnchor({ 0.5f,0.5f });
    clearTex_.SetTexcoordRect({ 0.0f,0.0f }, { 540.0f, 720.0f });
    clearTex_.SetScale(texScale);
    clearTex_.SetIsActive(true);

#pragma region スコア取得

	//
	const char dataName[] = "data";

	//インスタンス取得
	GlobalVariables* globalV = GlobalVariables::GetInstance();
	//グループの追加
	GlobalVariables::GetInstance()->CreateGroup(dataName);

	//limit
	std::string keyLimit = "Limit";
	//ボブかどうか
	std::string IsBob = "IsBob";
	
	//
	limitScore_ = globalV->GetIntvalue(dataName, keyLimit);
	isBob_ = globalV->GetIntvalue(dataName, IsBob);
#pragma endregion

    clearLimit_ = std::make_unique<ClearLimit>();
    clearLimit_->Initialize();

}

void Clear::OnUpdate() {

#ifdef _DEBUG
    ImGui::Begin("titletex");
    ImGui::DragFloat3("pos", &clearTrans_.translate.x);
    ImGui::End();
    clearTex_.SetPosition(clearTrans_.translate.GetXY());
#endif // _DEBUG

    clearLimit_->Update(limitScore_);

    //シーンチェンジ処理
    SceneChange();
}

void Clear::OnFinalize() {
}

void Clear::SceneChange() {
    //SPACEキーでゲームシーン
    if (input_->IsKeyTrigger(DIK_SPACE)) {
        //インスタンス取得
        SceneManager* sceneManager = SceneManager::GetInstance();
        //シーン設定
        sceneManager->ChangeScene<TitleScene>();
    }
}
