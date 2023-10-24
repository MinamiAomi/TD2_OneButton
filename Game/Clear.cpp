#include "Graphics/ResourceManager.h"
#include"Externals/ImGui/imgui.h"
#include"Clear.h"
#include"TitleScene.h"
#include"ImGame.h"

void Clear::OnInitialize() {
    input_ = Input::GetInstance();

    ResourceManager* resourceManager = ResourceManager::GetInstance();
    //仮画像
    const char textureName[] = "uvChecker";

    clearTrans_.translate = { 270.0f,500.0f,0.0f };

    Vector2 texScale = { 64.0f,64.0f };

    clearTex_.SetTexture(resourceManager->FindTexture(textureName));
    clearTex_.SetPosition(clearTrans_.translate.GetXY());
    clearTex_.SetAnchor({ 0.5f,0.5f });
    clearTex_.SetTexcoordRect({ 0.0f,0.0f }, { 64.0f, 64.0f });
    clearTex_.SetScale(texScale);
    clearTex_.SetIsActive(true);

}

void Clear::OnUpdate() {

#ifdef _DEBUG
    ImGui::Begin("titletex");
    ImGui::DragFloat3("pos", &clearTrans_.translate.x);
    ImGui::End();
    clearTex_.SetPosition(clearTrans_.translate.GetXY());
#endif // _DEBUG

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
