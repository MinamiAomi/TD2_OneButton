#include "DeferredRender.h"

#include "Core/Graphics.h"
#include "Core/Helper.h"
#include "Core/ShaderManager.h"
#include "Core/CommandContext.h"
#include "Core/SamplerManager.h"

#include "ModelRenderer.h"

void DeferredRender::Create(uint32_t width, uint32_t height) {
    // ベースカラーバッファ
    float baseColorClearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
    baseColorBuffer_.SetClearColor(baseColorClearColor);
    baseColorBuffer_.Create(L"GeometryPass BaseColorBuffer", width, height, DXGI_FORMAT_R8G8B8A8_UNORM);

    // 法線バッファ
    float normalClearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
    normalBuffer_.SetClearColor(normalClearColor);
    normalBuffer_.Create(L"GeometryPass NormalBuffer", width, height, DXGI_FORMAT_R32G32B32_FLOAT);
    // 深度バッファ
    depthBuffer_.Create(L"GeometryPass DepthBuffer", width, height, DXGI_FORMAT_D32_FLOAT);
    // 出力バッファ
    outputBuffer_.Create(L"DeferredRender OutputBuffer", width, height, DXGI_FORMAT_R8G8B8A8_UNORM);

    CreateRootSignature();
    CreateGeometryPipeline();
    CreateLightingPipeline();
}

void DeferredRender::Render(CommandContext& commandContext) {
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
    commandContext.SetPipelineState(geometryPassPipelineState_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // TODO: 不透明モデル描画を追加
    auto modelRenderers = ModelRenderer::GetInstanceList();
    for (auto& mr : modelRenderers) {
        if (mr->IsActive()) {
            mr->Draw(commandContext);
        }
    }

    commandContext.TransitionResource(baseColorBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    commandContext.TransitionResource(normalBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    commandContext.TransitionResource(depthBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    commandContext.SetRenderTarget(outputBuffer_.GetRTV());
    commandContext.ClearColor(outputBuffer_);
    commandContext.SetViewportAndScissorRect(0, 0, outputBuffer_.GetWidth(), outputBuffer_.GetHeight());

    commandContext.SetRootSignature(rootSignature_);
    commandContext.SetPipelineState(lightingPassPipelineState_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    struct Setting {
        uint32_t baseColorIndex;
        uint32_t normalIndex;
        uint32_t depthIndex;
        uint32_t numDirectionalLights;
    } setting{};
    setting.baseColorIndex = baseColorBuffer_.GetSRV().GetIndex();
    setting.normalIndex = normalBuffer_.GetSRV().GetIndex();
    setting.depthIndex = depthBuffer_.GetSRV().GetIndex();
    commandContext.SetDynamicConstantBufferView(0, sizeof(setting), &setting);
    commandContext.SetBindlessResource(2);
    commandContext.SetDescriptorTable(3, SamplerManager::PointClamp);
    
    // TODO: ライトを追加


    commandContext.Draw(3);
    commandContext.TransitionResource(outputBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void DeferredRender::CreateRootSignature() {
    CD3DX12_DESCRIPTOR_RANGE srvRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, BINDLESS_RESOURCE_MAX, 0, 1);
    CD3DX12_DESCRIPTOR_RANGE samplerRange(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParameter[5]{};
    rootParameter[0].InitAsConstantBufferView(0); // GeometryPassではTransformation, LightingPassではSetting
    rootParameter[1].InitAsConstantBufferView(1); // GeometryPassではMaterial
    rootParameter[2].InitAsDescriptorTable(1, &srvRange); // BindLessTexture
    rootParameter[3].InitAsDescriptorTable(1, &samplerRange); // サンプラー
    rootParameter[4].InitAsShaderResourceView(0); // LightingPassでDirectionalLight

    D3D12_ROOT_SIGNATURE_DESC rsDesc{};
    rsDesc.NumParameters = _countof(rootParameter);
    rsDesc.pParameters = rootParameter;
    rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    rootSignature_.Create(L"GeometryPass RootSignature", rsDesc);
}

void DeferredRender::CreateGeometryPipeline() {
    auto shaderManager = ShaderManager::GetInstance();
    auto vs = shaderManager->Compile(L"Resources/Shader/GeometryPassVS.hlsl", ShaderManager::kVertex);
    auto ps = shaderManager->Compile(L"Resources/Shader/GeometryPassPS.hlsl", ShaderManager::kPixel);

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
    geometryPassPipelineState_.Create(L"GeometryPass PipelineState", psDesc);
}

void DeferredRender::CreateLightingPipeline() {
    auto shaderManager = ShaderManager::GetInstance();
    auto vs = shaderManager->Compile(L"Resources/Shader/ScreenQuadVS.hlsl", ShaderManager::kVertex);
    auto ps = shaderManager->Compile(L"Resources/Shader/LightingPassPS.hlsl", ShaderManager::kPixel);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc{};
    psDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
    psDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());

    psDesc.RasterizerState = Helper::RasterizerDefault;
    psDesc.NumRenderTargets = 1;
    psDesc.RTVFormats[0] = outputBuffer_.GetFormat();
    psDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psDesc.SampleDesc.Count = 1;
    psDesc.BlendState = Helper::BlendDisable;
    lightingPassPipelineState_.Create(L"GeometryPass PipelineState", psDesc);
}
