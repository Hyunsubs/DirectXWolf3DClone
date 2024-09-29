#ifndef _RAYCASTER2D
#define _RAYCASTER2D

#include "value.fx"
#include "func.fx"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720

#define TILE_SIZE 64.f
#define ANGLE         radians(66)

// Ÿ�ϸ� üũ
#define AtlasTex            g_tex_0
#define IsAtlasBind         g_btex_0

#define AtlasMaxRow         g_int_0
#define AtlasMaxCol         g_int_1
#define TileSliceUV         g_vec2_0
#define TileColRow          g_vec2_1

StructuredBuffer<tRay2D> Ray2DBuffer : register(t18);
StructuredBuffer<tTileInfo> TileBuffer : register(t19);


// Vertex Shader
struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
};

VTX_OUT VS_RayCast2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
   
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;


    return output;
}

float4 PS_RayCast2D(VTX_OUT _in) : SV_Target
{
    
    float4 vColor = float4(0.4f, 0.4f, 0.4f, 1.f);

    tRay2D curRay = Ray2DBuffer[_in.vUV.x * SCREEN_WIDTH];
   
    float2 middle_ray_dir = Ray2DBuffer[SCREEN_WIDTH / 2].RayDir;
    float correction_angle = dot(curRay.RayDir, middle_ray_dir);
    float fLightFactor = saturate(300.f / curRay.RayDist);
    int lineheight = (int) (100000.f / curRay.RayDist) * cos(correction_angle);

    int drawStart = -lineheight / 2 + SCREEN_HEIGHT / 2;
    int drawEnd = lineheight / 2 + SCREEN_HEIGHT / 2;
    
    if (curRay.imgIdx != 0 && _in.vUV.y * SCREEN_HEIGHT >= drawStart && _in.vUV.y * SCREEN_HEIGHT <= drawEnd)
    {
               
        int row = (curRay.imgIdx - 1) / AtlasMaxCol;
        int col = (curRay.imgIdx - 1) % AtlasMaxCol;
       
        float2 vLeftTopUV = float2(col, row) * TileSliceUV;
        float fRenderX;
        float fRenderY;
        if (curRay.HitVert)
        {
            fRenderX = ((int) curRay.RayIntersection.x) % TILE_SIZE / TILE_SIZE;
            fRenderY = (_in.vUV.y * SCREEN_HEIGHT - drawStart) / lineheight;
        }
        else
        {
            fRenderX = ((int) curRay.RayIntersection.y) % TILE_SIZE / TILE_SIZE;
            fRenderY = (_in.vUV.y * SCREEN_HEIGHT - drawStart) / lineheight;
        }

       
        float2 vRenderUV = float2(fRenderX, fRenderY);
        float2 vUV = vLeftTopUV + frac(vRenderUV) * TileSliceUV;
        
        vColor = AtlasTex.Sample(g_sam_1, vUV);
        
        vColor *= fLightFactor;
    }
    
    else
    {
        if (_in.vUV.y * SCREEN_HEIGHT <= drawStart)
        {
            vColor = float4(0.15f, 0.15f, 0.15f, 0.f);
        }

        else
        {
            vColor = float4(0.45f, 0.45f, 0.45f, 0.f);
        }

    }

    return vColor;
}


#endif
