struct Transformation
{
    float4x4 worldMatrix;
    float4x4 viewProjMatrix;
    float3 cameraPosition;
};
ConstantBuffer<Transformation> transformation_ : register(b0);

struct Material
{
    float3 color;
};
ConstantBuffer<Material> material_ : register(b1);

struct DirectionalLight
{
    float3 direction;
    float intensity;
    float3 color;
};
ConstantBuffer<DirectionalLight> directionalLight_ : register(b2);

Texture2D<float4> texture_ : register(t0);
SamplerState sampler_ : register(s0);
Texture2D<float4> toonShadeTexture_ : register(t0, space1);
SamplerState toonShadeSampler_ : register(s0, space1);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
};

struct PSOutput
{
    float4 color : SV_TARGET0;
};

float3 ToonDiffuse(float3 normal, float3 lightDirection)
{
    float NdotL = dot(normalize(normal), -lightDirection);
    float2 toonShadeUV = float2(NdotL, 0.0f);
    float3 color = toonShadeTexture_.Sample(sampler_, toonShadeUV).xyz;
    return color;
}

PSOutput main(PSInput input)
{
    PSOutput output;
            
    float3 textureColor = texture_.Sample(sampler_, input.texcoord).xyz * material_.color;
    float3 shadeColor = ToonDiffuse(normalize(input.normal), directionalLight_.direction) * directionalLight_.color;
    output.color = textureColor * shadeColor;
    
    output.color.a = 1.0f;
    return output;
}