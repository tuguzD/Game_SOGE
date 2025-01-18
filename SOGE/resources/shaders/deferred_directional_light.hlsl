#include "world_position_from_depth.hlsl"

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

cbuffer Pipeline_ConstantBuffer : register(b0)
{
    float4x4 inv_projection;
    float4x4 inv_view;
    float3 view_position;
}

cbuffer DirectionalLight_ConstantBuffer : register(b1)
{
    float3 color;
    float intensity;
    float3 direction;
}

Texture2D<float> gBuffer_depth : register(t0);
Texture2D<float4> gBuffer_albedo : register(t1);
Texture2D<float4> gBuffer_normal : register(t2);

typedef VS_Output PS_Input;

float4 PSMain(PS_Input input) : SV_Target
{
    int3 gBuffer_coord = int3(input.position.xy, 0);

    float depth = gBuffer_depth.Load(gBuffer_coord);
    if (depth == 1.0f)
    {
        discard;
    }
    float3 world_position = WorldPositionFromDepth(input.clip_coord, depth, inv_projection, inv_view);
    float4 albedo = gBuffer_albedo.Load(gBuffer_coord);
    float3 normal = gBuffer_normal.Load(gBuffer_coord).xyz;

    float diffuse = max(0.0f, dot(normal, -direction));

    float3 to_view_direction = normalize(view_position - world_position);
    float3 reflect_direction = normalize(reflect(-direction, normal));
    float specular = pow(max(dot(-to_view_direction, reflect_direction), 0.0f), 64.0f);

    return albedo * float4(color, 1.0f) * intensity * (diffuse + specular);
}
