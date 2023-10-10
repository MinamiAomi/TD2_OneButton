struct Transformation
{
    float4x4 worldMatrix;
    float4x4 viewProjMatrix;
};

ConstantBuffer<Transformation> transformation_ : register(b0);

struct Outline
{
    float width;
    float3 color;
};
ConstantBuffer<Outline> outline_ : register(b0);

struct VSInput
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    float3 worldNormal = mul(input.normal, (float3x3) transformation_.worldMatrix);
    float4 worldPosition = mul(float4(input.position, 1.0f), transformation_.worldMatrix);
    worldPosition += normalize(worldNormal) * outline_.width;
    
    output.position = mul(worldPosition, transformation_.viewProjMatrix);
    
    return output;
}