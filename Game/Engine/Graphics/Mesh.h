#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <filesystem>

#include "Math/MathUtils.h"
#include "Core/GPUBuffer.h"
#include "Core/TextureResource.h"

class CommandContext;

class Model {
    friend class ModelRenderer;
public:
    struct Vertex {
        Vector3 position;
        Vector3 normal;
        Vector2 texcoord;
    };
    using Index = uint16_t;

    enum class Sampler {
        LinearWrap,
        LinearClamp,
        PointWrap,
        PointClamp
    };

    void Create(const std::filesystem::path& path);
    void SetSampler(Sampler sampler) { sampler_ = sampler; }

private:
    void LoadOBJFile(const std::filesystem::path& path);
    void LoadMTLFile(const std::filesystem::path& path);

    struct Texture {
        std::filesystem::path fullpath;
        std::string name;
        TextureResource resource;
    };
    struct Mesh {
        uint32_t vertexOffset;
        uint32_t indexOffset;
        uint32_t indexCount;
        uint32_t materialIndex;
    };
    struct Material {
        std::string name;
        Vector4 color;
        uint32_t textureIndex;
    };
    std::vector<Vertex> vertices_;
    std::vector<Index> indices_;
    std::unique_ptr<GPUBuffer> vertexBuffer_;
    std::unique_ptr<GPUBuffer> indexBuffer_;
    std::vector<Mesh> meshes_;
    std::vector<Material> materials_;
    std::vector<Texture> textures_;
    Sampler sampler_ = Sampler::LinearWrap;
};