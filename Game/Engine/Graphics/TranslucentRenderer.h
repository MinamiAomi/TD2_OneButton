#pragma once
#pragma once

#include "Core/RootSignature.h"
#include "Core/PipelineState.h"
#include "Core/TextureResource.h"

class CommandContext;
class ColorBuffer;
class DepthBuffer;
class Camera;

struct TranslucentRootIndex {
    enum Index {
        Scene = 0,
        Instance,
        Texture,
        Sampler,

        NumParameters
    };
};

class TranslucentRenderer {
public:

    void Initialize(const ColorBuffer& colorBuffer, const DepthBuffer& depthBuffer);
    void Render(CommandContext& commandContext, const Camera& camera);

private:
    void InitializeRootSignature();
    void InitializePipeline(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat);

    RootSignature rootSignature_;
    PipelineState pipelineState_;
};