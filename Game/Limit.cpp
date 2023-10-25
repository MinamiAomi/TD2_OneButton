#include"Limit.h"

#include "Graphics/ResourceManager.h"
#include"Externals/ImGui/imgui.h"

void Limit::Initialize() {

    ResourceManager* resourceManager = ResourceManager::GetInstance();
    const char textureName[] = "number";
    resourceManager->FindTexture(textureName)->SetInterpolation(Texture::Interpolation::Point);

    Vector2 texScale = { texScale_,texScale_ };

    //画像の初期化
    for (int i = 0; i < digitNum; i++) {
        numSprite_[i].spirte = std::make_unique<Sprite>();
        numSprite_[i].spirte->SetTexture(resourceManager->FindTexture(textureName));

        numSprite_[i].spirte->SetAnchor({ 0.5f,0.5f });
        numSprite_[i].spirte->SetScale(texScale);
        numSprite_[i].spirte->SetIsActive(true);
        numSprite_[i].spirte->SetColor({ 1.0f,1.0f,1.0f, 0.3f });

    }

    const char meterName[] = "limit";
    resourceManager->FindTexture(meterName)->SetInterpolation(Texture::Interpolation::Point);

    meterSprite_.spirte = std::make_unique<Sprite>();
    meterSprite_.spirte->SetTexture(resourceManager->FindTexture(meterName));
    meterSprite_.spirte->SetAnchor({ 0.5f,0.5f });
    meterSprite_.spirte->SetScale(mScale_);
    meterSprite_.spirte->SetTexcoordRect({ 0.0f,0.0f }, { 150.0f,200.0f });
    meterSprite_.spirte->SetIsActive(true);
    meterSprite_.spirte->SetColor({ 1.0f,1.0f,1.0f, 0.3f });

}

void Limit::Update(int limit) {

#ifdef _DEBUG
    ImGui::Begin("limit");
    ImGui::DragFloat2("pos", &center_.x);
    ImGui::DragFloat2("mpos", &mPos_.x);
    ImGui::DragFloat2("mScale", &mScale_.x); 
    ImGui::End();
   
    meterSprite_.spirte->SetScale(mScale_);
#endif // _DEBUG


    if (limit != 0) {



        //一桁目取得
        num_[0] = limit % 10;

        //二桁目の取得
        num_[1] = limit - num_[0];


        int count = 0;
        while (true) {
            count++;
            num_[1] -= 10;
            //二桁目の数字を作成
            if (num_[1] == 0) {

                //三桁目がある場合
                if (count >= 10) {
                    //二桁目の数字を入力
                    num_[1] = count % 10;
                    numSprite_[1].spirte->SetIsActive(true);
                    //値を代入
                    num_[2] = count;
                    //初期化
                    count = 0;
                    while (true) {
                        count++;
                        num_[2] -= 10;
                        //三桁目の数字を取得
                        if (num_[2] == 0) {
                            num_[2] = count;
                            numSprite_[2].spirte->SetIsActive(true);

                            numSprite_[0].position = center_;
                            numSprite_[0].position.x += texScale_ / 2.0f;

                            numSprite_[1].position = center_;

                            numSprite_[0].position = center_;
                            numSprite_[0].position.x -= texScale_ / 2.0f;

                            break;
                        }
                    }
                }
                else {//ない場合
                    num_[1] = count;
                    numSprite_[1].spirte->SetIsActive(true);
                    numSprite_[2].spirte->SetIsActive(false);

                    //座標処理
                    numSprite_[0].position = center_;
                    numSprite_[0].position.x += texScale_ / 4.0f;

                    numSprite_[1].position = center_;
                    numSprite_[1].position.x -= texScale_ / 4.0f;

                    break;
                }


            }//以下一桁の場合
            else if (num_[1] < 0.0f) {

                //数字を中心点に
                numSprite_[0].position = center_;

                numSprite_[1].spirte->SetIsActive(false);
                numSprite_[2].spirte->SetIsActive(false);


                break;
            }
        }

    }
    else {
        num_[0] = 0;
        //数字を中心点に
        numSprite_[0].position = center_;

        numSprite_[1].spirte->SetIsActive(false);
        numSprite_[2].spirte->SetIsActive(false);

    }


    for (int i = 0; i < digitNum; i++) {
        numSprite_[i].spirte->SetPosition(numSprite_[i].position);
        numSprite_[i].spirte->SetTexcoordRect({ texSize * num_[i],0.0f }, { texSize,texSize });
    }

    //座標設定
    meterSprite_.position = numSprite_[0].position;
    meterSprite_.position.x = numSprite_[0].position.x + texScale_ / 2.0f;
    meterSprite_.position += mPos_;
    meterSprite_.spirte->SetPosition(meterSprite_.position);
}
