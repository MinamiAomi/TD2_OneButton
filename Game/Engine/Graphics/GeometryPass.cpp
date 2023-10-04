#include "GeometryPass.h"

#include "Core/Helper.h"
#include "Core/ShaderManager.h"
#include "Core/CommandContext.h"

void GeometryPass::Create(uint32_t width, uint32_t height) {
    float baseColorClearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
    baseColorBuffer_.SetClearColor(baseColorClearColor);
    baseColorBuffer_.Create(L"GeometryPass BaseColorBuffer", width, height, DXGI_FORMAT_R8G8B8A8_UNORM);


    float normalClearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
    normalBuffer_.SetClearColor(normalClearColor);
    normalBuffer_.Create(L"GeometryPass NormalBuffer", width, height, DXGI_FORMAT_R32G32B32_FLOAT);

    depthBuffer_.Create(L"GeometryPass DepthBuffer", width, height, DXGI_FORMAT_D32_FLOAT);

    CD3DX12_DESCRIPTOR_RANGE srvRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    CD3DX12_DESCRIPTOR_RANGE samplerRange(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParameter[10]{};
    rootParameter[0].InitAsConstantBufferView(0);
    rootParameter[1].InitAsConstantBufferView(1);
    rootParameter[2].InitAsDescriptorTable(1, &srvRange);
    rootParameter[3].InitAsDescriptorTable(1, &samplerRange);

    D3D12_ROOT_SIGNATURE_DESC rsDesc{};
    rsDesc.NumParameters = _countof(rootParameter);
    rsDesc.pParameters = rootParameter;
    rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    rootSignature_.Create(L"GeometryPass RootSignature", rsDesc);

    auto shaderManager = ShaderManager::GetInstance();
    auto vs = shaderManager->Compile(L"Resources/Shader/GeometryVS.hlsl", ShaderManager::kVertex);
    auto ps = shaderManager->Compile(L"Resources/Shader/GeometryPS.hlsl", ShaderManager::kPixel);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc{};
    psDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
    psDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());
    D3D12_INPUT_ELEMENT_DESC inputElements[] = {
          { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
          { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
          { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
    inputLayoutDesc.pInputElementDescs = inputElements;
    inputLayoutDesc.NumElements = _countof(inputElements);
    psDesc.InputLayout = inputLayoutDesc;

    psDesc.RasterizerState = Helper::RasterizerDefault;
    psDesc.NumRenderTargets = 2;
    psDesc.RTVFormats[0] = baseColorBuffer_.GetFormat();
    psDesc.RTVFormats[1] = normalBuffer_.GetFormat();
    psDesc.DSVFormat = depthBuffer_.GetFormat();
    psDesc.DepthStencilState = Helper::DepthStateReadWrite;
    psDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psDesc.SampleDesc.Count = 1;
    psDesc.BlendState = Helper::BlendDisable;
    pipelineState_.Create(L"GeometryPass PipelineState", psDesc);
}

void GeometryPass::Draw(CommandContext& commandContext) {
    commandContext.TransitionResource(baseColorBuffer_, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext.TransitionResource(normalBuffer_, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext.TransitionResource(depthBuffer_, D3D12_RESOURCE_STATE_DEPTH_WRITE);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvs[] = {
        baseColorBuffer_.GetRTV(),
        normalBuffer_.GetRTV()
    };
    commandContext.SetRenderTargets(_countof(rtvs), rtvs, depthBuffer_.GetDSV());
    commandContext.ClearColor(baseColorBuffer_);
    commandContext.ClearColor(normalBuffer_);
    commandContext.ClearDepth(depthBuffer_);
    commandContext.SetViewportAndScissorRect(0, 0, baseColorBuffer_.GetWidth(), baseColorBuffer_.GetHeight());

    commandContext.SetRootSignature(rootSignature_);
    commandContext.SetPipelineState(pipelineState_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



    commandContext.TransitionResource(baseColorBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    commandContext.TransitionResource(normalBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    commandContext.TransitionResource(depthBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
