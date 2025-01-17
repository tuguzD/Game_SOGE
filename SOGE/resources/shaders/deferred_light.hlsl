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

Texture2D<float> g_depth : register(t0);
Texture2D<float4> g_albedo : register(t1);

typedef VS_Output PS_Input;

float4 PSMain(PS_Input input) : SV_Target
{
    float4 color = g_albedo.Load(int3(input.position.xy, 0));
    return color;
}
