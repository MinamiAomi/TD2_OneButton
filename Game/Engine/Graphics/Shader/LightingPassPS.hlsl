// バインドレステクスチャ
#define BINDLESS_TEXTURE_SPACE space1
Texture2D<float4> bindlessTexture_[] : register(t0, BINDLESS_TEXTURE_SPACE);
// サンプラー
SamplerState sampler_ : register(s0);

// GBufferのインデックス, 各種ライトの数 
struct Setting
{
    uint baseColorIndex;
    uint normalIndex;
    uint depthIndex;
    uint numDirectionalLights;
};
ConstantBuffer<Setting> setting_ : register(b0);

// 平行光源
struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};
StructuredBuffer<DirectionalLight> directionalLights_ : register(t0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PSOutput
{
    float3 color : SV_TARGET0;
};

PSOutput main(PSInput input)
{
    PSOutput output;

    // テクスチャを持ってくる
    Texture2D<float4> baseColorGBuffer = bindlessTexture_[setting_.baseColorIndex];
    Texture2D<float4> normalGBuffer = bindlessTexture_[setting_.normalIndex];
    Texture2D<float4> depthGBuffer = bindlessTexture_[setting_.depthIndex];
    
    
    output.color = baseColorGBuffer.Sample(sampler_, input.texcoord);
    
    return output;
}