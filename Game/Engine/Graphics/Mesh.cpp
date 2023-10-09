#include "Mesh.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Core/Graphics.h"
#include "Core/CommandContext.h"

void Model::Create(const std::filesystem::path& path) {

    LoadOBJFile(path);

    vertexBuffer_ = std::make_unique<GPUBuffer>();
    indexBuffer_ = std::make_unique<GPUBuffer>();
    vertexBuffer_->Create(L"Model VertexBuffer", sizeof(vertices_[0]) * vertices_.size());
    indexBuffer_->Create(L"Model IndexBuffer", sizeof(indices_[0]) * indices_.size());
    
    CommandContext commandContext;
    commandContext.Create();

    commandContext.CopyBuffer(*vertexBuffer_, vertexBuffer_->GetBufferSize(), vertices_.data());
    commandContext.CopyBuffer(*indexBuffer_, indexBuffer_->GetBufferSize(), indices_.data());
    for (auto& texture : textures_) {
        texture.resource.CreateFromWICFile(commandContext, texture.fullpath);
    }
    auto& commandQueue = Graphics::GetInstance()->GetCommandQueue();
    commandContext.Close();
    commandQueue.Excute(commandContext);
    commandQueue.Signal();
    commandQueue.WaitForGPU();
}
void Model::LoadOBJFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    assert(file.is_open());

    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;

    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    std::unordered_map<std::string, uint16_t> vertexDefinitionMap;

    std::string line;
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream iss(line);
        iss >> identifier;

        if (identifier == "#") {
            continue;
        }
        else if (identifier == "mtllib") {
            std::string materialFileName;
            iss >> materialFileName;
            LoadMTLFile(path.parent_path() / materialFileName);
        }
        else if (identifier == "v") {
            Vector3& position = positions.emplace_back();
            iss >> position.x >> position.y >> position.z;
            // position.z = -position.z;
        }
        else if (identifier == "vn") {
            Vector3& normal = normals.emplace_back();
            iss >> normal.x >> normal.y >> normal.z;
            // normal.z = -normal.z;
        }
        else if (identifier == "vt") {
            Vector2& texcoord = texcoords.emplace_back();
            iss >> texcoord.x >> texcoord.y;
            texcoord.y = 1.0f - texcoord.y;
        }
        else if (identifier == "usemtl") {
            std::string materialName;
            iss >> materialName;
            auto iter = std::find_if(materials_.begin(), materials_.end(),
                [&](const auto& material) {
                    return material.name == materialName;
                });
            if (iter != materials_.end()) {
                Mesh mesh{};
                mesh.indexOffset = uint32_t(indices.size());
                if (!meshes_.empty()) {
                    meshes_.back().indexCount = uint32_t(indices.size()) - meshes_.back().indexOffset;
                }
                mesh.materialIndex = uint32_t(std::distance(materials_.begin(), iter));
                meshes_.emplace_back(mesh);
            }
            else {
                assert(false);
            }
        }
        else if (identifier == "f") {
            // 面の頂点を取得
            std::vector<std::string> vertexDefinitions;
            while (true) {
                std::string vertexDefinition;
                iss >> vertexDefinition;
                if (vertexDefinition.empty()) {
                    break;
                }
                vertexDefinitions.emplace_back(std::move(vertexDefinition));
            }
            assert(vertexDefinitions.size() > 2);
            std::vector<uint16_t> face(vertexDefinitions.size());
            for (uint32_t i = 0; i < vertexDefinitions.size(); ++i) {
                // 頂点が登録済み
                if (vertexDefinitionMap.contains(vertexDefinitions[i])) {
                    face[i] = vertexDefinitionMap[vertexDefinitions[i]];
                }
                else {
                    std::istringstream viss(vertexDefinitions[i]);
                    uint32_t elementIndices[3]{};
                    bool useElement[3]{};
                    for (uint32_t j = 0; j < 3; ++j) {
                        std::string index;
                        std::getline(viss, index, '/');
                        if (!index.empty()) {
                            elementIndices[j] = static_cast<uint32_t>(std::stoi(index)) - 1;
                            useElement[j] = true;
                        }
                    }
                    auto& vertex = vertices.emplace_back();
                    vertex.position = positions[elementIndices[0]];
                    if (useElement[1]) {
                        vertex.texcoord = texcoords[elementIndices[1]];
                    }
                    if (useElement[2]) {
                        vertex.normal = normals[elementIndices[2]];
                    }
                    face[i] = vertexDefinitionMap[vertexDefinitions[i]] = static_cast<uint16_t>(vertices.size() - 1);
                }
            }

            for (uint32_t i = 0; i < face.size() - 2; ++i) {
                indices.emplace_back(Index(face[0]));
                indices.emplace_back(Index(face[i + 1ull]));
                indices.emplace_back(Index(face[i + 2ull]));
            }
        }
    }

    if (!meshes_.empty()) {
        meshes_.back().indexCount = uint32_t(indices.size()) - meshes_.back().indexOffset;
    }

    file.close();

    vertices_ = std::move(vertices);
    indices_ = std::move(indices);
}
void Model::LoadMTLFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    assert(file.is_open());

    std::string line;
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream iss(line);
        iss >> identifier;

        // コメントをスキップ
        if (identifier == "#") {
            continue;
        }
        else if (identifier == "newmtl") {
            std::string materialName;
            iss >> materialName;
            materials_.emplace_back();
            materials_.back().name = materialName;
        }
        else if (identifier == "map_Kd") {
            std::string textureName;
            iss >> textureName;

            auto iter = std::find_if(textures_.begin(), textures_.end(),
                [&](const auto& texture) {
                    return texture.name == textureName;
                });

            // 読み込み済みテクスチャ
            if (iter != textures_.end()) {
                materials_.back().textureIndex = uint32_t(std::distance(textures_.begin(), iter));
            }
            else {
                materials_.back().textureIndex = uint32_t(textures_.size());
                textures_.emplace_back();
                textures_.back().fullpath = path.parent_path() / textureName;
                textures_.back().name = textureName;
            }

            materials_.back().color = Vector4::one;

        }
        else if (identifier == "Kd") {
            Vector4 color;
            iss >> color.x >> color.y >> color.z;
            color.w = 1.0f;
            materials_.back().color = color;
        }
    }
}