#include "ToonRenderer.h"

#include "Core/Helper.h"
#include "Core/ShaderManager.h"

void ToonRenderer::Initialize(const ColorBuffer& colorBuffer, const DepthBuffer& depthBuffer) {
}

void ToonRenderer::Render(CommandContext& commandContext, const Camera& camera) {
}

void ToonRenderer::InitializeOutlinePass(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat) {

    CD3DX12_ROOT_PARAMETER rootParameters[2]{};
    rootParameters[0].InitAsConstantBufferView(0);
    rootParameters[1].InitAsConstantBufferView(1);

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.NumParameters = _countof(rootParameters);
    rootSignatureDesc.pParameters = rootParameters;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    outlineRootSignature_.Create(L"Outline RootSignature", rootSignatureDesc);
    
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
    
    pipelineStateDesc.pRootSignature = outlineRootSignature_;

    D3D12_INPUT_ELEMENT_DESC inputElements[] = {
         { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
         { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };
    D3D12_INPUT_LAYOUT_DESC inputLayout{};
    inputLayout.NumElements = _countof(inputElements);
    inputLayout.pInputElementDescs = inputElements;
    pipelineStateDesc.InputLayout = inputLayout;

    auto shaderManager = ShaderManager::GetInstance();
    auto vs = shaderManager->Compile(L"Engine/Graphics/Shader/ToonVS.hlsl", ShaderManager::kVertex);
    auto ps = shaderManager->Compile(L"Engine/Graphics/Shader/ToonPS.hlsl", ShaderManager::kPixel);
    
    pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
    pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());

    pipelineStateDesc.BlendState = Helper::BlendDisable;
    pipelineStateDesc.DepthStencilState = Helper::DepthStateReadWrite;
    pipelineStateDesc.RasterizerState = Helper::RasterizerDefault;
    // 前面カリング
    pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
    pipelineStateDesc.NumRenderTargets = 1;
    pipelineStateDesc.RTVFormats[0] = rtvFormat;
    pipelineStateDesc.DSVFormat = dsvFormat;
    pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipelineStateDesc.SampleDesc.Count = 1;
    outlinePipelineState_.Create(L"Outline PipelineState", pipelineStateDesc);
}

void ToonRenderer::InitializeToonPass(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat)
{
}
