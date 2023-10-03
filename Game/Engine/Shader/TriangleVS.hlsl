struct VSInput {
    float3 position : POSITION0;
    float4 color : COLOR0;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.position = float4(input.position.xyz, 1.0f);
    output.color = input.color;
    return output;
}