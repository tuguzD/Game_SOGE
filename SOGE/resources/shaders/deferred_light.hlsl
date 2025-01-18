struct VS_Input
{
    float3 position : POSITION0;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 clip_coord : TEXCOORD0;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    output.position = float4(input.position, 1.0f);
    output.clip_coord = input.position.xy;

    return output;
}

cbuffer PS_ConstantBuffer : register(b0)
{
    float4x4 inv_projection;
    float4x4 inv_view;
}

Texture2D<float> gBuffer_depth : register(t0);
Texture2D<float4> gBuffer_albedo : register(t1);

typedef VS_Output PS_Input;

float3 WorldPositionFromDepth(float2 clip_coord, float depth)
{
    float4 clip_position = float4(clip_coord, depth, 1.0f);

    float4 view_position = mul(inv_projection, clip_position);
    view_position /= view_position.w;

    float4 world_position = mul(inv_view, view_position);
    return world_position.xyz;
}

float4 PSMain(PS_Input input) : SV_Target
{
    int3 gBuffer_position = int3(input.position.xy, 0);

    float depth = gBuffer_depth.Load(gBuffer_position);
    if (depth == 1.0f)
    {
        discard;
    }
    float3 world_position = WorldPositionFromDepth(input.clip_coord, depth);

    float4 color = float4(world_position, 1.0f);
    // float4 color = gBuffer_albedo.Load(gBuffer_position);
    return color;
}
