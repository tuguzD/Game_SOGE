cbuffer Pipeline_ConstantBuffer : register(b0)
{
    float4x4 viewProjection;
}

cbuffer Object_ConstantBuffer : register(b1)
{
    float4x4 model;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float3 emissive;
    float shininess;
}

struct VS_Input
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float3 color : COLOR0;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float3 color : COLOR0;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    output.position = mul(mul(viewProjection, model), float4(input.position, 1.0f));
    output.normal = normalize(mul(model, float4(input.normal, 0.0f)).xyz);
    output.color = input.color;

    return output;
}

typedef VS_Output PS_Input;

struct PS_Output
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
};

PS_Output PSMain(PS_Input input)
{
    PS_Output output = (PS_Output)0;

    output.albedo = float4(input.color, 1.0f);
    output.normal = float4(input.normal, 1.0f);

    return output;
}
