// バインドレステクスチャ
#define BINDLESS_TEXTURE_SPACE space1
Texture2D<float4> bindlessTexture_[] : register(t0, BINDLESS_TEXTURE_SPACE);
SamplerState sampler_ : register(s0);

struct Material
{
    uint baseColorTextureInedx;
    
    float3 baseColorFactor;
};
ConstantBuffer<Material> material_ : register(b1);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
};

struct PSOutput
{
    float3 baseColor : SV_TARGET0;
    float3 normal : SV_TARGET1;
};

PSOutput main(PSInput input)
{
    PSOutput output;

    Texture2D<float4> baseColorTexture = bindlessTexture_[material_.baseColorTextureInedx];
    
    output.baseColor = material_.baseColorFactor * baseColorTexture.Sample(sampler_, input.texcoord).rgb;
    output.normal = normalize(input.normal) * 0.5f + 0.5f;
    
    return output;
}