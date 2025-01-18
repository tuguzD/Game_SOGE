struct VS_Input
{
    float3 position : POSITION0;
};

struct VS_Output
{
    float4 position : SV_POSITION;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    output.position = float4(input.position, 1.0f);

    return output;
}

cbuffer PS_ConstantBuffer : register(b0)
{
    float3 color;
    float intensity;
}

Texture2D<float> gBuffer_depth : register(t0);
Texture2D<float4> gBuffer_albedo : register(t1);

typedef VS_Output PS_Input;

float4 PSMain(PS_Input input) : SV_Target
{
    int3 gBuffer_coord = int3(input.position.xy, 0);

    float depth = gBuffer_depth.Load(gBuffer_coord);
    if (depth == 1.0f)
    {
        discard;
    }

    float4 albedo = gBuffer_albedo.Load(gBuffer_coord);
    return albedo * float4(color, 1.0f) * intensity;
}
