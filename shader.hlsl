//**
// 定数バッファ
//**
cbuffer ConstantBuffer : register(b0)
{
    matrix WorldViewProjection;
}

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);
//=============================================================================
// 頂点シェーダ
//=============================================================================
void VS(in float4 inPosition : POSITION0,
        in float4 inDiffuse : COLOR0,
        in float2 inTexCoord : TEXCOORD0,
        out float4 outPosition : SV_POSITION,
        out float4 outDiffuse : COLOR0,
        out float2 outTexCoord : TEXCOORD0)
{
    outPosition = mul(inPosition, WorldViewProjection);
    
    
    outTexCoord = inTexCoord;
    outDiffuse = inDiffuse;
}


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PS(in float4 inPosition : POSITION0,
        in float4 inDiffuse : COLOR0,
        out float4 outDiffuse : SV_Target,
        in float2 inTexCoord : TEXCOORD0)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
    //outDiffuse = float4(1.0f, 0.0f, 1.0f, 1.0f);
}