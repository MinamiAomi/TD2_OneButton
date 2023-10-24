#pragma once

#include <memory>
#include <vector>

#include "Core/GPUBuffer.h"
#include "Core/UploadBuffer.h"
#include "Core/TextureResource.h"

#include "ModelLoader.h"

class ToonModel {
    friend class ToonRenderer;
    friend class TranslucentRenderer;
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
        UploadBuffer constantBuffer;
        std::shared_ptr<Texture> texture;
    };
    struct Mesh {
        UploadBuffer vertexBuffer;
        UploadBuffer indexBuffer;
        uint32_t indexCount;
        std::shared_ptr<Material> material;
    };

    std::vector<Mesh> meshes_;
};

class ToonModelInstance {
    friend class ToonRenderer;
    friend class TranslucentRenderer;
public:
    enum class Pass : uint8_t {
        Opaque,     // 不透明
        Translucent // 半透明
    };

    ToonModelInstance();
    virtual ~ToonModelInstance();

    void SetModel(const std::shared_ptr<ToonModel>& model) { model_ = model; }
    void SetWorldMatrix(const Matrix4x4& worldMatrix) { worldMatrix_ = worldMatrix; }
    void SetIsActive(bool isActive) { isActive_ = isActive; }
    void SetColor(const Vector3& color) { color_ = color; }
    void SetAlpha(float alpha) { alpha_ = alpha; }
    void SetOutlineWidth(float width) { outlineWidth_ = width; }
    void SetOutlineColor(const Vector3& color) { outlineColor_ = color; }
    void SetUseOutline(bool useOutline) { useOutline_ = useOutline; }
    void SetIsLighting(bool isLighting) { isLighting_ = isLighting; }
    void SetPass(Pass pass) { pass_ = pass; }

    bool IsActive() const { return isActive_; }

private:
    static std::list<ToonModelInstance*> instanceList_;

    ToonModelInstance(const ToonModelInstance&) = delete;
    ToonModelInstance& operator=(const ToonModelInstance&) = delete;
    ToonModelInstance(ToonModelInstance&&) = delete;
    ToonModelInstance& operator=(ToonModelInstance&&) = delete;

    std::shared_ptr<ToonModel> model_;
    Matrix4x4 worldMatrix_;
    Vector3 color_ = Vector3::one;
    float alpha_ = 1.0f;
    Vector3 outlineColor_;
    float outlineWidth_ = {0.02f};
    bool useOutline_ = false;
    bool isLighting_ = true;
    bool isActive_ = true;
    Pass pass_ = Pass::Opaque;
};