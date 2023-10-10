struct Outline
{
    float width;
    float3 color;
};
ConstantBuffer<Outline> outline_ : register(b0);

struct PSOutput
{
    float4 color : SV_TARGET0;
};

PSOutput main()
{
    PSOutput output;

    output.color = float4(outline_.color, 1.0f);
    
    return output;
}