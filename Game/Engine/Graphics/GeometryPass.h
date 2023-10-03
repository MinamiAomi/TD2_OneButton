#pragma once

#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Core/RootSignature.h"
#include "Core/PipelineState.h"

class CommandContext;

class GeometryPass {
public:

    void Create(uint32_t width, uint32_t height);
    void Draw(CommandContext& commandContext);

    ColorBuffer& GetBaseColorBuffer() { return baseColorBuffer_; }
    ColorBuffer& GetNormalBuffer() { return normalBuffer_; }
    DepthBuffer& GetDepthBuffer() { return depthBuffer_; }

private:
    PipelineState pipelineState_;
    RootSignature rootSignature_;

    ColorBuffer baseColorBuffer_;
    ColorBuffer normalBuffer_;
    DepthBuffer depthBuffer_;
};