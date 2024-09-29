#ifndef _MISCRENDER
#define _MISCRENDER
#include "value.fx"
#include "func.fx"


// Vertex Shader
struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
};

VTX_OUT VS_MiscRender(VTX_IN _in) : SV_Position
{
    VTX_OUT output;
    
    output.vPosition = float4(_in.vPos, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_MiscRender(VTX_OUT _in) : SV_Position
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    
    
    
    return vColor;
}




#endif
