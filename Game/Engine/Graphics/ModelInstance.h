#pragma once
#include <memory>
#include <list>

#include "Math/MathUtils.h"

class Model;

class ModelInstance {
public:
    static std::list<ModelInstance*>& GetInstanceList() { return instanceLists_; }

    ModelInstance();
    virtual ~ModelInstance();

    void SetModel(const std::shared_ptr<Model>& model) { model_ = model; }
    void SetWorldMatrix(const Matrix4x4& worldMatrix) { worldMatrix_ = worldMatrix; }
    void SetIsActive(bool isActive) { isActive_ = isActive; }
    bool IsActive() const { return isActive_; }

private:
    static std::list<ModelInstance*> instanceLists_;

    std::shared_ptr<Model> model_;
    Matrix4x4 worldMatrix_;
    float outlineWidth_;
    Vector3 outlineColor_;
    bool useOutline_;
    bool isActive_;
};