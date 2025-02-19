﻿#include "world_position_from_depth.hlsl"

cbuffer Pipeline_ConstantBuffer : register(b0)
{
    float4x4 viewProjection;

    float4x4 inv_projection;
    float4x4 inv_view;
    float3 view_position;
}

struct Attenuation
{
    float3 const_factor;
    float3 linear_factor;
    float3 quadratic_factor;
};

cbuffer PointLight_ConstantBuffer : register(b1)
{
    float4x4 model;

    float3 color;
    float intensity;
    float3 position;
    Attenuation attenuation;
}

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

    float4 result = mul(mul(viewProjection, model), float4(input.position, 1.0f));
    output.position = result;
    output.clip_coord = result.xy;

    return output;
}

Texture2D<float> gBuffer_depth : register(t0);
Texture2D<float4> gBuffer_albedo : register(t1);
Texture2D<float4> gBuffer_normal : register(t2);
Texture2D<float4> gBuffer_diffuse : register(t3);
Texture2D<float4> gBuffer_specularShininess : register(t4);

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
    float3 diffuse = gBuffer_diffuse.Load(gBuffer_coord).xyz;
    float3 specular = gBuffer_specularShininess.Load(gBuffer_coord).xyz;
    float shininess = gBuffer_specularShininess.Load(gBuffer_coord).w;

    float3 to_light = world_position - position;
    float to_light_distance = length(to_light);
    float3 final_attenuation = attenuation.const_factor + attenuation.linear_factor * to_light_distance +
                               attenuation.quadratic_factor * to_light_distance * to_light_distance;
    final_attenuation = (length(final_attenuation) > 0.0f) ? final_attenuation : float3(1.0f, 1.0f, 1.0f);

    float3 direction = normalize(to_light);
    float lightDiffuse = max(0.0f, dot(normal, -direction));

    float3 to_view_direction = normalize(view_position - world_position);
    float3 reflect_direction = normalize(reflect(-direction, normal));
    float lightSpecular = pow(max(dot(-to_view_direction, reflect_direction), 0.0f), shininess);

    return albedo *
           float4(color * intensity * (diffuse * lightDiffuse + specular * lightSpecular) / final_attenuation, 1.0f);
}
