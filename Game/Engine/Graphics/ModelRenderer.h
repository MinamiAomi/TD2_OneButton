#pragma once
#include "Renderer.h"

#include <memory>
#include <list>

#include "Math/MathUtils.h"

class Model;

class ModelRenderer :
    public Renderer {
public:
    static std::list<ModelRenderer*> GetInstanceList() { return instanceLists_; }

    ModelRenderer();
    virtual ~ModelRenderer();

    void Draw(CommandContext& commandContext);

    void SetModel(const std::shared_ptr<Model>& model) { model_ = model; }
    void SetWorldMatrix(const Matrix4x4& worldMatrix) { worldMatrix_ = worldMatrix; }

private:
    static std::list<ModelRenderer*> instanceLists_;

    std::shared_ptr<Model> model_;
    Matrix4x4 worldMatrix_;
};