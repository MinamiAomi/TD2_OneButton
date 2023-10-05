#pragma once

#include <vector>
#include <memory>
#include <filesystem>

#include "Core/UploadBuffer.h"
#include "Core/Texture.h"
#include "Math/MathUtils.h"
#include "Math/Camera.h"

class Model {
public:
    enum class SamplerType {
        kLinearWrap,
        kLinearClamp,
        kPointWrap,
        kPointClamp
    };
    struct Vertex {
        Vector3 position;
        Vector3 normal;
        Vector2 texcoord;
    };


    void CreateFromObj(const std::filesystem::path& path);
    void Draw(CommandContext& commandContext, const Matrix4x4& world, const Camera& camera, bool isLighting = true);

    void SetSamplerType(SamplerType samplerType) { samplerType_ = samplerType; }

private:
    struct Mesh {
        uint32_t vertexOffset;
        uint32_t indexOffset;
        uint32_t indexCount;
        uint32_t materialIndex;
    };
    struct Material {
        std::string name;
        Vector3 baseColorFactor;
        std::unique_ptr<Texture> baseColorTexture;
    };

    void LoadMTLFile(const std::filesystem::path& path);

    UploadBuffer vertexBuffer_;
    D3D12_VERTEX_BUFFER_VIEW vbView_{};
    UploadBuffer indexBuffer_;
    D3D12_INDEX_BUFFER_VIEW ibView_{};
    std::vector<Mesh> meshes_;
    std::vector<Material> materials_;
    SamplerType samplerType_ = SamplerType::kLinearWrap;
};