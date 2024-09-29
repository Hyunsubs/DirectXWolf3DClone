#ifndef _LINE
#define _LINE

#include "value.fx"
#include "struct.fx"


StructuredBuffer<tRay2D> Ray2DBuffer : register(t16);

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    uint InstID : SV_InstanceID;
};


struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


VS_OUT VS_LineShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 크기 -> 이동
    float3 vWorldPos = _in.vPos * float3(1.f, 1.f, 1.f) + float3(Ray2DBuffer[_in.InstID].RayPos, 1.f);
        
    output.vPosition = mul(mul(float4(vWorldPos, 1.f), matView), matProj);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_LineShader(VS_OUT _in) : SV_Target
{
    
    
    return float4(0.f, 1.f, 0.f, 1.f);
}




#endif