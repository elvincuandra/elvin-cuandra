#include "common.hlsl"

struct VS_IN
{
    float3 Position : POSITION0;
	float3 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    int4 BoneIndex : BONEINDEX;
    float4 BoneWeight : BONEWEIGHT;
};

struct VS_OUT
{
    float4 Position : SV_POSITION0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};


VS_OUT main( VS_IN vin) 
{
    VS_OUT vout;
    float4x4 combMtx = (float4x4) 0;
    for (int i = 0; i < 4; i++){
        combMtx += BoneMatrix[vin.BoneIndex[i]] * vin.BoneWeight[i];
    }
    
    float4 pos;
    pos = mul(combMtx, float4(vin.Position, 1.f));
    vin.Position = pos;
    
    float4 normal;
    normal = float4(vin.Normal.xyz, 0.f);
    
    combMtx._41 = 0.f;
    combMtx._42 = 0.f;
    combMtx._43 = 0.f;
    combMtx._44 = 1.f;
    
    normal = mul(combMtx, float4(vin.Normal, 1.f));
    normalize(normal);
    vin.Normal = normal;
    
    matrix wvp;
    wvp = mul(world, view);
    wvp = mul(wvp, proj);
    
    float4 worldNormal, Normal;
    Normal = float4(vin.Normal.xyz, 1);
    
    worldNormal = mul(Normal, world);
    worldNormal = normalize(worldNormal);
    
    vout.Diffuse = vin.Diffuse * material.Diffuse;
    vout.Diffuse += vin.Diffuse * material.Ambient;
    vout.Diffuse += material.Emission;
    vout.Diffuse.a = vin.Diffuse.a * material.Diffuse.a;
    
    vout.Position = mul(float4(vin.Position, 1), wvp);
    vout.TexCoord = vin.TexCoord;
    return vout;
}