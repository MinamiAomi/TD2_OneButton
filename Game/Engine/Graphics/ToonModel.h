#pragma once

#include <memory>
#include <vector>

#include "Core/GPUBuffer.h"
#include "Core/TextureResource.h"

#include "ModelLoader.h"

class ToonModel {
    friend class ToonRenderer;
public:
    using Vertex = ModelData::Vertex;
    using Index = ModelData::Index;

    void Create(const ModelData& modelData);
    
private:
    struct Texture {
        TextureResource textureResource;
        DescriptorHandle sampler;
    };
    struct Material {
        GPUBuffer constantBuffer;
        std::shared_ptr<Texture> texture;
    };
    struct Mesh {
        GPUBuffer vertexBuffer;
        GPUBuffer indexBuffer;
        uint32_t indexCount;
        std::shared_ptr<Material> material;
    };

    std::vector<Mesh> meshes_;
};

class ToonModelInstance {
    friend class ToonRenderer;
public:
    static const std::list<ToonModelInstance*>& GetInstanceList() { return instanceLists_; }

    ToonModelInstance();
    virtual ~ToonModelInstance();

    void SetModel(const std::shared_ptr<ToonModel>& model) { model_ = model; }
    void SetWorldMatrix(const Matrix4x4& worldMatrix) { worldMatrix_ = worldMatrix; }
    void SetIsActive(bool isActive) { isActive_ = isActive; }
    void SetOutlineWidth(float width) { outlineWidth_ = width; }
    void SetOutlineColor(const Vector3& color) { outlineColor_ = color; }
    void SetUseOutline(bool useOutline) { useOutline_ = useOutline; }

    bool IsActive() const { return isActive_; }

private:
    static std::list<ToonModelInstance*> instanceLists_;

    ToonModelInstance(const ToonModelInstance&) = delete;
    ToonModelInstance& operator=(const ToonModelInstance&) = delete;
    ToonModelInstance(ToonModelInstance&&) = delete;
    ToonModelInstance& operator=(ToonModelInstance&&) = delete;

    std::shared_ptr<ToonModel> model_;
    Matrix4x4 worldMatrix_;
    float outlineWidth_ = {0.01f};
    Vector3 outlineColor_;
    bool useOutline_ = true;
    bool isActive_;
};