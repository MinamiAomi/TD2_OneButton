struct Scene {
    float4x4 viewProjMatrix;
};
ConstantBuffer<Scene> scene_ : register(b0);

struct Instance {
    float4x4 worldMatrix;
    float3 color;
    float alpha;
};
ConstantBuffer<Instance> instance_ : register(b1);

Texture2D<float4> texture_ : register(t0);
SamplerState sampler_ : register(s0);

struct PSInput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PSOutput {
    float4 color : SV_TARGET0;
};

PSOutput main(PSInput input) {
    // テクスチャの色
    float4 textureColor = texture_.Sample(sampler_, input.texcoord);
    
    PSOutput output;
    output.color.rgb = textureColor.rgb * instance_.color;
    output.color.a = textureColor.a * instance_.alpha;
    
    if (output.color.a <= 0.0f) {
        discard;
    }
    
    return output;
}