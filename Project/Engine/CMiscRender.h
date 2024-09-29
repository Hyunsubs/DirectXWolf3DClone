#pragma once
#include "CRenderComponent.h"


class CMiscRender :
    public CComponent
{
private:
    Vec3         m_CurrentPos;
    vector<CGameObject*> m_miscObjects;
    
    CGameObject* m_RenderObj;

    float m_Fov;

public:
    virtual void Init() override;
    virtual void FinalTick() override;

    void FindMiscObjects();
    void CheckPassedObjects();

    float GetFOV() { return m_Fov; }
    void SetFOV(float _Fov) { m_Fov = _Fov; }

public:
    CLONE(CMiscRender)
    CMiscRender();
    ~CMiscRender();

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
};

