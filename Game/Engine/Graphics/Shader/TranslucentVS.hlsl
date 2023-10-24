struct Scene {
    float4x4 viewProjMatrix;
};
ConstantBuffer<Scene> scene_ : register(b0);

struct Instance {
    float4x4 worldMatrix;
};
ConstantBuffer<Instance> instance_ : register(b1);

struct VSInput {
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), mul(instance_.worldMatrix, scene_.viewProjMatrix));
    output.texcoord = input.texcoord;
    return output;
}