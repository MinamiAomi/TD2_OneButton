#pragma once

#include "Core/Graphics.h"
#include "Core/SwapChain.h"
#include "Core/CommandContext.h"
#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Bloom.h"
#include "DeferredRender.h"

class RenderManager {
public:

    static RenderManager* GetInstance();

    void Render();

private:
    RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;

    void InitializePostEffect();

    Graphics* graphics_ = nullptr;
    SwapChain swapChain_;
    CommandContext commandContexts_[SwapChain::kNumBuffers];

    ColorBuffer mainColorBuffer_;
    DepthBuffer mainDepthBuffer_;
    RootSignature postEffectRootSignature_;
    PipelineState postEffectPipelineState_;

    DeferredRender deferredRender;
    Bloom bloom;
};