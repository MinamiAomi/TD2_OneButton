#pragma once

#include "Core/RootSignature.h"
#include "Core/PipelineState.h"
#include "Core/TextureResource.h"

class CommandContext;
class ColorBuffer;
class DepthBuffer;
class Camera;

class ToonRenderer {
public:

    void Initialize(const ColorBuffer& colorBuffer, const DepthBuffer& depthBuffer);
    void Render(CommandContext& commandContext, const Camera& camera);

private:
    RootSignature outlineRootSignature_;
    RootSignature outlinePipelineState_;
    
    RootSignature toonRootSignature_;
    PipelineState toonPipelineState_;

    TextureResource toonShadeTexture_;
};