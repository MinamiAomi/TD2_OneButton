#pragma once

#include "Math/Camera.h"
#include "Core/Graphics.h"
#include "Core/SwapChain.h"
#include "Core/CommandContext.h"
#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Bloom.h"
#include "ToonRenderer.h"
#include "PostEffect.h"
#include "Timer.h"

class RenderManager {
public:

    static RenderManager* GetInstance();

    void Initialize();
    void Finalize();
    void Render();

    void SetCamera(const Camera& camera) { camera_ = &camera; }
    void SetFPS(uint32_t fps) { fps_ = fps; }

private:
    RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;

    Graphics* graphics_ = nullptr;
    SwapChain swapChain_;
    CommandContext commandContexts_[SwapChain::kNumBuffers];

    ColorBuffer mainColorBuffer_;
    DepthBuffer mainDepthBuffer_;

    ToonRenderer toonRenderer_;
    Bloom bloom_;
    PostEffect postEffect_;

    const Camera* camera_ = nullptr;

    Timer timer_;
    uint32_t fps_ = 60;
};