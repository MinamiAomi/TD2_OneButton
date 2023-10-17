

Texture2D<float4> texture_ : register(t0);
SamplerState sampler_ : register(s0);

struct PSInput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PSOutput {
    float4 color : SV_TARGET0;
};

//float3 LinearToSRGB(float3 color)
//{
//    float3 sqrt1 = sqrt(color);
//    float3 sqrt2 = sqrt(sqrt1);
//    float3 sqrt3 = sqrt(sqrt2);
//    float3 srgb = 0.662002687 * sqrt1 + 0.684122060 * sqrt2 - 0.323583601 * sqrt3 - 0.0225411470 * color;
//    return srgb;
//}
    
PSOutput main(PSInput input) {
    PSOutput output;
    
    float2 texcoord = input.texcoord;
    
    //float len = length(texcoord);
    //float2 dir = texcoord / len;
    //texcoord = dir * pow(len, 2.0f);
    
    //uint2 size = 0;
    //texture_.GetDimensions(size.x, size.y);
    

    //float2 offset = float2(1.0f / size.x, 0.0f);
    //output.color.r = texture_.Sample(sampler_, texcoord).r;
    //output.color.g = texture_.Sample(sampler_, texcoord + offset).g;
    //output.color.b = texture_.Sample(sampler_, texcoord - offset).b;
    
    output.color = texture_.Sample(sampler_, texcoord);
    //output.color.xyz = LinearToSRGB(output.color.xyz);
        
    return output;
}