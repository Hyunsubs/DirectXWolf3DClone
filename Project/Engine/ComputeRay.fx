#ifndef _COMPUTE_RAY
#define _COMPUTE_RAY

#include "value.fx"
#include "func.fx"

#define WINDOW_WIDTH g_Resolution.x

RWStructuredBuffer<tRay2D> RayBuffer : register(u3);


[numthreads(1024, 1, 1)]
void CS_ComputeRay(int3 _ID : SV_DispatchThreadID)
{

    
}


#endif