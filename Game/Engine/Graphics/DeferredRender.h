#pragma once

#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Core/RootSignature.h"
#include "Core/PipelineState.h"

class CommandContext;

class DeferredRender {
public:

    void Create(uint32_t width, uint32_t height);
    void Render(CommandContext& commandContext);

    ColorBuffer& GetBaseColorBuffer() { return baseColorBuffer_; }
    ColorBuffer& GetNormalBuffer() { return normalBuffer_; }
    DepthBuffer& GetDepthBuffer() { return depthBuffer_; }

    ColorBuffer& GetOutputBuffer() { return outputBuffer_; }

private:
    void CreateRootSignature();
    void CreateGeometryPipeline();
    void CreateLightingPipeline();

    PipelineState geometryPassPipelineState_;
    PipelineState lightingPassPipelineState_;
    RootSignature rootSignature_;

    ColorBuffer baseColorBuffer_;
    ColorBuffer normalBuffer_;
    DepthBuffer depthBuffer_;

    ColorBuffer outputBuffer_;
};