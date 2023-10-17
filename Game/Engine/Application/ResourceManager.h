#pragma once

#include <string>
#include <map>
#include <memory>

class ToonModel;

class ResourceManager {
public:
    static ResourceManager* GetInstance();

    void AddModel(const std::string& name, const std::shared_ptr<ToonModel>& model) { modelMap_.emplace(std::make_pair(name, model)); }
    std::shared_ptr<ToonModel> FindModel(const std::string& name) const { return modelMap_.at(name); }

private:
    std::map<std::string, std::shared_ptr<ToonModel>> modelMap_;
};