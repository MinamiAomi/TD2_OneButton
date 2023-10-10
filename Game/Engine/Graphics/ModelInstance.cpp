#include "ModelInstance.h"

std::list<ModelInstance*> ModelInstance::instanceLists_;

ModelInstance::ModelInstance() {
    instanceLists_.emplace_back(this);
}

ModelInstance::~ModelInstance() {
    auto iter = std::find(instanceLists_.begin(), instanceLists_.end(), this);
    if (iter != instanceLists_.end()) {
        instanceLists_.erase(iter);
    }
}
