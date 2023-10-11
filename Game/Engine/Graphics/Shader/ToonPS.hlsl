#include "Lighting.hlsli"

struct Scene {
    float4x4 viewProjMatrix;
    float3 cameraPosition;
};
ConstantBuffer<Scene> scene_ : register(b0);

struct Material {
    float3 diffuse;
    float3 specular;
};
ConstantBuffer<Material> material_ : register(b2);

ConstantBuffer<DirectionalLight> directionalLight_ : register(b3);

Texture2D<float4> texture_ : register(t0);
SamplerState sampler_ : register(s0);
Texture2D<float4> toonShadeTexture_ : register(t0, space1);
SamplerState toonShadeSampler_ : register(s0, space1);

struct PSInput {
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
};

struct PSOutput {
    float4 color : SV_TARGET0;
};

float3 ToonDiffuse(float3 normal, float3 lightDirection) {
    float t = -dot(normal, lightDirection);
    float2 toonShadeUV = float2(t, 0.0f);
    float3 color = toonShadeTexture_.Sample(sampler_, toonShadeUV).rgb * material_.diffuse;
    return color;
}

float3 ToonSpecular(float3 normal, float3 pixelToCamera, float3 lightDirection) {
    const float shininess = 4.0f;
    const float threshold = 0.5f;
    float3 reflectVec = reflect(lightDirection, normal);
    float t = pow(saturate(dot(reflectVec, lightDirection)), shininess);
    float3 color = step(t, threshold) * material_.specular;
    return color;
}

PSOutput main(PSInput input) {
    // 位置
    float3 position = input.worldPosition;
    // 法線
    float3 normal = normalize(input.normal);
    // ピクセルからカメラへのベクトル 
    float3 pixelToCamera = normalize(scene_.cameraPosition - position);
    
    
    // テクスチャの色
    float3 textureColor = texture_.Sample(sampler_, input.texcoord).rgb;
    
    // 拡散反射
    float3 diffuse = ToonDiffuse(normal, directionalLight_.direction);
    // 鏡面反射
    float3 specular = ToonSpecular(normal, pixelToCamera, directionalLight_.direction);
    // シェーディングによる色
    float3 shadeColor = (diffuse + specular) * directionalLight_.color;
    
    
    PSOutput output;
    output.color.rgb = textureColor * shadeColor;
    output.color.a = 1.0f;
    
    return output;
}