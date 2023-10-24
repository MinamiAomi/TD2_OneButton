#include "Graphics/ResourceManager.h"
#include "Externals/ImGui/imgui.h"
#include "GlobalVariables.h"
#include "Clear.h"
#include "TitleScene.h"
#include "ImGame.h"

void Clear::OnInitialize() {
    input_ = Input::GetInstance();

#pragma region 画像初期化

    // 背景
    SpriteInitialize(backGroundTex_, "clearBackGround", { 540.0f, 720.0f });
    backGroundTransform_.translate = center;
    backGroundTex_.SetPosition(backGroundTransform_.translate.GetXY());
    // YouWin
    SpriteInitialize(youWinTex_, "youWin", { 400.0f, 64.0f });
    youWinTransform_.translate = center;
    youWinTex_.SetPosition(youWinTransform_.translate.GetXY());
    //// GoodBye
    //SpriteInitialize(goodByeTex_, "goodBye", { 540.0f, 720.0f });
    //goodByeTransform_.translate = center;
    //goodByeTex_.SetPosition(goodByeTransform_.translate.GetXY());
    //// Bob
    //SpriteInitialize(bobTex_, "bob", { 540.0f, 720.0f });
    //bobTransform_.translate = center;
    //bobTex_.SetPosition(bobTransform_.translate.GetXY());
    //// Michael
    //SpriteInitialize(michaelTex_, "michael", { 540.0f, 720.0f });
    //michaelTransform_.translate = center;
    //michaelTex_.SetPosition(michaelTransform_.translate.GetXY());
    //// スコアバーの線
    //SpriteInitialize(scoreBarLineTex_, "scoreBarLine", { 540.0f, 720.0f });
    //scoreBarLineTransform_.translate = center;
    //scoreBarLineTex_.SetPosition(scoreBarLineTransform_.translate.GetXY());
    //// スコアバーの円
    //SpriteInitialize(scoreBarCircleTex_, "scoreBarCircle", { 540.0f, 720.0f });
    //scoreBarCircleTransform_.translate = center;
    //scoreBarCircleTex_.SetPosition(scoreBarCircleTransform_.translate.GetXY());
    //// 仮の下端絵
    //SpriteInitialize(tmpBottomTex_, "tmpBottom", { 540.0f, 720.0f });
    //tmpBottomTransform_.translate = center;
    //tmpBottomTex_.SetPosition(tmpBottomTransform_.translate.GetXY());

#pragma endregion

#pragma region スコア取得

    const char dataName[] = "data";

    // インスタンス取得
    GlobalVariables* globalV = GlobalVariables::GetInstance();
    // グループの追加
    GlobalVariables::GetInstance()->CreateGroup(dataName);

    std::string keyLimit = "Limit";
    std::string IsBob = "IsBob";

    // 各パラメータの取得
    limitScore_ = globalV->GetIntvalue(dataName, keyLimit);
    isBob_ = globalV->GetIntvalue(dataName, IsBob);

#pragma endregion

    clearLimit_ = std::make_unique<ClearLimit>();
    clearLimit_->Initialize(limitScore_);

}

void Clear::OnUpdate() {

#ifdef _DEBUG
    ImGui::Begin("titletex");
    ImGui::DragFloat3("pos", &backGroundTransform_.translate.x);
    ImGui::End();
    backGroundTex_.SetPosition(backGroundTransform_.translate.GetXY());
#endif // _DEBUG

    clearLimit_->Update();

    //シーンチェンジ処理
    SceneChange();
}

void Clear::OnFinalize() {
}

void Clear::SpriteInitialize(Sprite& sprite, const char textureName[], Vector2 size) {
    ResourceManager* resourceManager = ResourceManager::GetInstance();
    sprite.SetTexture(resourceManager->FindTexture(textureName));
    sprite.SetAnchor({ 0.5f,0.5f });
    sprite.SetTexcoordRect({ 0.0f,0.0f }, size);
    sprite.SetScale(size);
    sprite.SetIsActive(true);
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
