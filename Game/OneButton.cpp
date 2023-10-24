#include "OneButton.h"

#include <fstream>
#include <cassert>

#include "Scene/SceneManager.h"
#include "TitleScene.h"
#include "ImGame.h"
#include "Clear.h"
#include "Graphics/ResourceManager.h"
#include "Externals/nlohmann/json.hpp"
#include "Graphics/Sprite.h"
#include "Graphics/ToonModel.h"

#include"GlobalVariables.h"

void OneButton::OnInitialize() {
    GlobalVariables::GetInstance()->LoadFiles();


    SceneManager* sceneManager = SceneManager::GetInstance();
    //シーン設定
    sceneManager->ChangeScene<InGame>();

    LoadResource();
}

void OneButton::OnFinalize() {
}

void OneButton::LoadResource() {
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    std::ifstream file("Resources/LoadResource.json");
    assert(file.is_open());
    nlohmann::json json;
    file >> json;
    file.close();

    nlohmann::json& models = json.at("Models");
    for (auto& model : models) {
        std::string name = model.at("Name");
        std::string path = model.at("Path");
        auto toonModel = std::make_shared<ToonModel>();
        toonModel->Create(ModelData::LoadObjFile(path));
        resourceManager->AddModel(name, toonModel);
    }

    nlohmann::json& images = json.at("Images");
    for (auto& image : images) {
        std::string name = image.at("Name");
        std::string path = image.at("Path");
        auto texture = std::make_shared<Texture>();
        texture->Load(path);
        resourceManager->AddTexture(name, texture);
    }
}
