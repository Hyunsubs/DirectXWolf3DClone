#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;
class CLight2D;
class CStructuredBuffer;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr)
private:
    vector<CCamera*>        m_vecCam;
    CCamera*                m_EditorCamera;

    CGameObject*            m_DebugObject;
    list<tDebugShapeInfo>   m_DebugShapeList;

    //Misc Render
    vector<tMiscRender>     m_vecMiscRender;

    //Ray Buffer
    CStructuredBuffer* m_RayBuffer;

    // Light
    vector<CLight2D*>       m_vecLight2D;
    CStructuredBuffer*      m_Light2DBuffer;

    // PostProcess
    Ptr<CTexture>           m_PostProcessTex;


public:
    void RegisterCamera(CCamera* _Cam, int _CamPriority);
    void RegisterEditorCamera(CCamera* _Cam) { m_EditorCamera = _Cam; }
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.push_back(_Info); }
    void RegisterLight2D(CLight2D* _Light) { m_vecLight2D.push_back(_Light); }
    void AddMiscRender(const tMiscRender& _Info) { m_vecMiscRender.push_back(_Info); }
    void SetRayBuffer(CStructuredBuffer* _Buffer) { m_RayBuffer = _Buffer; }
    void SortMiscObjects();
    void RenderMisc();
    void PostProcessCopy();

private:
    void RenderStart();
    void Clear();
    void RenderDebugShape();


public:
    void Init();
    void Tick();
};

