#include "ModelRenderer.h"

std::list<ModelRenderer*> ModelRenderer::instanceLists_;

ModelRenderer::ModelRenderer() {
    instanceLists_.emplace_back(this);
}

ModelRenderer::~ModelRenderer() {
    auto iter = std::find(instanceLists_.begin(), instanceLists_.end(), this);
    if (iter != instanceLists_.end()) {
        instanceLists_.erase(iter);
    }
}
