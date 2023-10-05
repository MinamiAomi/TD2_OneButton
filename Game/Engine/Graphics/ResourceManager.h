#pragma once

#include <memory>
#include <map>
#include <filesystem>

class Model;
class Texture;

class ResourceManager {
public:
    static ResourceManager* GetInstance();

    std::shared_ptr<Model> LoadModel(const std::string& name, const std::filesystem::path& path);
    std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::filesystem::path& path);

    std::shared_ptr<Model> FindModel(const std::string& name) const { return models_.at(name); }
    std::shared_ptr<Texture> FindTexture(const std::string& name) const { return textures_.at(name); }


private:
    template<class T>
    using ResourceList = std::map<std::string, std::shared_ptr<T>>;

    ResourceList<Model> models_;
    ResourceList<Texture> textures_;
};