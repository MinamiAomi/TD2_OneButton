#include "TranslucentRenderer.h"

#include "Core/Helper.h"
#include "Core/ShaderManager.h"
#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Core/CommandContext.h"
#include "Math/Camera.h"
#include "ToonModel.h"

const wchar_t kTranslucentVertexShadedr[] = L"Engine/Graphics/Shader/TranslucentVS.hlsl";
const wchar_t kTranslucentPixelShadedr[] = L"Engine/Graphics/Shader/TranslucentPS.hlsl";

void TranslucentRenderer::Initialize(const ColorBuffer& colorBuffer, const DepthBuffer& depthBuffer) {
    InitializeRootSignature();
    InitializePipeline(colorBuffer.GetFormat(), depthBuffer.GetFormat());
}

void TranslucentRenderer::Render(CommandContext& commandContext, const Camera& camera) {
    struct SceneConstant {
        Matrix4x4 viewProjectionMatrix;
    };

    struct InstanceConstant {
        Matrix4x4 worldMatrix;
        Vector3 color;
        float alpha;
    };

    auto& instanceList = ToonModelInstance::instanceList_;

    // 描画
    commandContext.SetRootSignature(rootSignature_);
    commandContext.SetPipelineState(pipelineState_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    SceneConstant scene{};
    scene.viewProjectionMatrix = camera.GetViewProjectionMatrix();
    commandContext.SetDynamicConstantBufferView(TranslucentRootIndex::Scene, sizeof(scene), &scene);

    // ライティングを使用した描画
    for (auto& instance : instanceList) {
        if (instance->IsActive() && instance->model_) {
            // ライティングしない
            if (instance->pass_ == ToonModelInstance::Pass::Translucent) {

                InstanceConstant data;
                data.worldMatrix = instance->worldMatrix_;
                data.color = instance->color_;
                data.alpha = instance->alpha_;
                commandContext.SetDynamicConstantBufferView(TranslucentRootIndex::Instance, sizeof(data), &data);

                for (auto& mesh : instance->model_->meshes_) {
                    commandContext.SetDescriptorTable(TranslucentRootIndex::Texture, mesh.material->texture->textureResource.GetSRV());
                    commandContext.SetDescriptorTable(TranslucentRootIndex::Sampler, mesh.material->texture->sampler);

                    D3D12_VERTEX_BUFFER_VIEW vbv{};
                    vbv.BufferLocation = mesh.vertexBuffer.GetGPUVirtualAddress();
                    vbv.SizeInBytes = (UINT)mesh.vertexBuffer.GetBufferSize();
                    vbv.StrideInBytes = (UINT)sizeof(ModelData::Vertex);
                    commandContext.SetVertexBuffer(0, vbv);
                    D3D12_INDEX_BUFFER_VIEW ibv{};
                    ibv.BufferLocation = mesh.indexBuffer.GetGPUVirtualAddress();
                    ibv.SizeInBytes = (UINT)mesh.indexBuffer.GetBufferSize();
                    ibv.Format = (sizeof(ModelData::Index) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
                    commandContext.SetIndexBuffer(ibv);
                    commandContext.DrawIndexed(mesh.indexCount);
                }
            }
        }
    }
}

void TranslucentRenderer::InitializeRootSignature() {
    CD3DX12_DESCRIPTOR_RANGE srvRange[2]{};
    srvRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_DESCRIPTOR_RANGE samplerRange{};
    samplerRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParameters[TranslucentRootIndex::NumParameters]{};
    rootParameters[TranslucentRootIndex::Scene].InitAsConstantBufferView(0);
    rootParameters[TranslucentRootIndex::Instance].InitAsConstantBufferView(1);
    rootParameters[TranslucentRootIndex::Texture].InitAsDescriptorTable(1, &srvRange[0]);
    rootParameters[TranslucentRootIndex::Sampler].InitAsDescriptorTable(1, &samplerRange);


    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.NumParameters = _countof(rootParameters);
    rootSignatureDesc.pParameters = rootParameters;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignature_.Create(L"Translucent RootSignature", rootSignatureDesc);
}

void TranslucentRenderer::InitializePipeline(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat) {
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};

    pipelineStateDesc.pRootSignature = rootSignature_;

    D3D12_INPUT_ELEMENT_DESC inputElements[] = {
         { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
         { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
         { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };
    D3D12_INPUT_LAYOUT_DESC inputLayout{};
    inputLayout.NumElements = _countof(inputElements);
    inputLayout.pInputElementDescs = inputElements;
    pipelineStateDesc.InputLayout = inputLayout;

    auto shaderManager = ShaderManager::GetInstance();
    auto vs = shaderManager->Compile(kTranslucentVertexShadedr, ShaderManager::kVertex);
    auto ps = shaderManager->Compile(kTranslucentPixelShadedr, ShaderManager::kPixel);

    pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
    pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());

    pipelineStateDesc.BlendState = Helper::BlendAdditive;
    pipelineStateDesc.DepthStencilState = Helper::DepthStateReadOnly;
    pipelineStateDesc.RasterizerState = Helper::RasterizerDefault;
    pipelineStateDesc.NumRenderTargets = 1;
    pipelineStateDesc.RTVFormats[0] = rtvFormat;
    pipelineStateDesc.DSVFormat = dsvFormat;
    pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipelineStateDesc.SampleDesc.Count = 1;
    pipelineState_.Create(L"Translucent PipelineState", pipelineStateDesc);
}
