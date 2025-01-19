float3 WorldPositionFromDepth(float2 clip_coord, float depth, in float4x4 inv_projection, in float4x4 inv_view)
{
    float4 clip_position = float4(clip_coord, depth, 1.0f);

    float4 view_position = mul(inv_projection, clip_position);
    view_position /= view_position.w;

    float4 world_position = mul(inv_view, view_position);
    return world_position.xyz;
}
