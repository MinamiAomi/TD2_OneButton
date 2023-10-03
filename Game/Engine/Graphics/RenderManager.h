#pragma once

#include "Core/Graphics.h"
#include "Core/SwapChain.h"
#include "Core/CommandContext.h"
#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Bloom.h"

class RenderManager {
public:

    static RenderManager* GetInstance();

    void Initialize();
    void Reset();
    void BeginRender();
    void EndRender();
    void Shutdown();

    SwapChain& GetSwapChain() { return swapChain_; }
    CommandContext& GetCommandContext() { return commandContexts_[swapChain_.GetBufferIndex()]; }
    Bloom& GetBloom() { return bloom; }

    DXGI_FORMAT GetSwapChainRTVFormat() const { return swapChain_.GetColorBuffer().GetFormat(); }
    DXGI_FORMAT GetMainBufferRTVFormat() const { return mainColorBuffer_.GetFormat(); }
    DXGI_FORMAT GetMainDepthDSVFormat() const { return mainDepthBuffer_.GetFormat(); }

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
    Bloom bloom;
};