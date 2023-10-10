#pragma once

#include "Math/Camera.h"
#include "Core/Graphics.h"
#include "Core/SwapChain.h"
#include "Core/CommandContext.h"
#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Bloom.h"

class ModelInstance;

class RenderManager {
public:

    static RenderManager* GetInstance();

    void Render();

    void SetCamera(const Camera& camera) { camera_ = &camera; }

private:
    RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;

    void InitializePostEffect();

    void RenderModels();

    Graphics* graphics_ = nullptr;
    SwapChain swapChain_;
    CommandContext commandContexts_[SwapChain::kNumBuffers];

    RootSignature modelRootSignature_;
    PipelineState modelPipelineState_;

    ColorBuffer mainColorBuffer_;
    DepthBuffer mainDepthBuffer_;
    RootSignature postEffectRootSignature_;
    PipelineState postEffectPipelineState_;

    Bloom bloom;

    const Camera* camera_;
};