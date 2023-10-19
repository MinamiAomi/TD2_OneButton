#include "OneButton.h"

#include <fstream>
#include <cassert>

#include "Scene/SceneManager.h"
#include "TitleScene.h"
#include "ImGame.h"
#include "Graphics/ResourceManager.h"
#include "Externals/nlohmann/json.hpp"

void OneButton::OnInitialize() {
    SceneManager* sceneManager = SceneManager::GetInstance();
    //シーン設定
    sceneManager->ChangeScene<InGame>();

    LoadResource();
}

void OneButton::OnFinalize() {
}

void OneButton::LoadResource() {
    ResourceManager* resourceManager = ResourceManager::GetInstance();
    std::shared_ptr<ToonModel> toonModel;
    std::shared_ptr<Texture> texture;

    std::ifstream file("Resources/LoadResource.json");
    assert(file.is_open());
    nlohmann::json json;
    file >> json;
    file.close();

    nlohmann::json& models = json.at("Models");
    for (auto& model : models) {
        std::string name = model.at("Name");
        std::string path = model.at("Path");
        toonModel = std::make_shared<ToonModel>();
        toonModel->Create(ModelData::LoadObjFile(path));
        resourceManager->AddModel(name, toonModel);
    }

}
