#include "ToonRenderer.h"

#include "Core/Helper.h"
#include "Core/ShaderManager.h"
#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Core/CommandContext.h"

#include "ModelInstance.h"

const wchar_t kOutlineVertexShadedr[] = L"Engine/Graphics/Shader/OutlineVS.hlsl";
const wchar_t kOutlinePixelShadedr[] = L"Engine/Graphics/Shader/OutlinePS.hlsl";
const wchar_t kToonVertexShadedr[] = L"Engine/Graphics/Shader/ToonPS.hlsl";
const wchar_t kToonPixelShadedr[] = L"Engine/Graphics/Shader/ToonPS.hlsl";
const wchar_t kToonShadeTexture[] = L"Engine/Graphics/Resource/ToonShade.png";

void ToonRenderer::Initialize(const ColorBuffer& colorBuffer, const DepthBuffer& depthBuffer) {
    InitializeOutlinePass(colorBuffer.GetFormat(), depthBuffer.GetFormat());
    InitializeToonPass(colorBuffer.GetFormat(), depthBuffer.GetFormat());
    toonShadeTexture_.CreateFromWICFile(kToonShadeTexture);
}

void ToonRenderer::Render(CommandContext& commandContext, const Camera& camera) {

    auto& instances = ModelInstance::GetInstanceList();

    commandContext.SetRootSignature(outlineRootSignature_);
    commandContext.SetPipelineState(outlinePipelineState_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    




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
    auto vs = shaderManager->Compile(kOutlineVertexShadedr, ShaderManager::kVertex);
    auto ps = shaderManager->Compile(kOutlinePixelShadedr, ShaderManager::kPixel);

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

void ToonRenderer::InitializeToonPass(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat) {
    CD3DX12_DESCRIPTOR_RANGE srvRange[2]{};
    srvRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    srvRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 1);

    CD3DX12_DESCRIPTOR_RANGE samplerRange{};
    samplerRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParameters[6]{};
    rootParameters[0].InitAsConstantBufferView(0);
    rootParameters[1].InitAsConstantBufferView(1);
    rootParameters[2].InitAsConstantBufferView(2);
    rootParameters[3].InitAsDescriptorTable(1, &srvRange[0]);
    rootParameters[4].InitAsDescriptorTable(1, &srvRange[1]);
    rootParameters[5].InitAsDescriptorTable(1, &samplerRange);

    CD3DX12_STATIC_SAMPLER_DESC staticSamplers[1]{};
    staticSamplers[0].Init(0, D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    staticSamplers[0].RegisterSpace = 1;

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.NumParameters = _countof(rootParameters);
    rootSignatureDesc.pParameters = rootParameters;
    rootSignatureDesc.NumStaticSamplers = _countof(staticSamplers);
    rootSignatureDesc.pStaticSamplers = staticSamplers;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    toonRootSignature_.Create(L"Toon RootSignature", rootSignatureDesc);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};

    pipelineStateDesc.pRootSignature = toonRootSignature_;

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
    auto vs = shaderManager->Compile(kToonVertexShadedr, ShaderManager::kVertex);
    auto ps = shaderManager->Compile(kToonPixelShadedr, ShaderManager::kPixel);

    pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
    pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());

    pipelineStateDesc.BlendState = Helper::BlendDisable;
    pipelineStateDesc.DepthStencilState = Helper::DepthStateReadWrite;
    pipelineStateDesc.RasterizerState = Helper::RasterizerDefault;
    // 前面カリング
    pipelineStateDesc.NumRenderTargets = 1;
    pipelineStateDesc.RTVFormats[0] = rtvFormat;
    pipelineStateDesc.DSVFormat = dsvFormat;
    pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipelineStateDesc.SampleDesc.Count = 1;
    toonPipelineState_.Create(L"Outline PipelineState", pipelineStateDesc);
}
