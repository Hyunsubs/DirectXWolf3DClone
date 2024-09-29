#pragma once
#include "CComponent.h"


class CRayCaster2D :
    public CComponent
{
private:
    Vec4 m_CurrentPos;
    Vec3 m_CurrentDir;
    Vec3 m_CurrentRot;

    CGameObject* m_TargetTileMap;

    class CStructuredBuffer* m_Ray2DBuffer;
    vector<tRay2D> m_Rays;

    int m_RayAmount;
    bool m_IsMiniMap;

    float m_Fov;
    Vec2 m_curIntersection;

public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void SetTargetTileMap(CGameObject* _Target) { m_TargetTileMap = _Target; }
    void SetMiniMap(bool _minimap) { m_IsMiniMap = _minimap; }
    CGameObject* GetTargetTileMap() { return m_TargetTileMap; }

    void SetRayAmount(int _Amount);
    void SetFOV(float _RadianFOV) { m_Fov = _RadianFOV; }

    float GetFOV() { return m_Fov; }

    int GetRayAmount() { return m_RayAmount; }
    Vec2 GetCurrentIntersection() { return m_curIntersection; }
    
    bool IsMiniMap() { return m_IsMiniMap; }
    CStructuredBuffer* GetRayBuffer() { return m_Ray2DBuffer; }
   
public:
    CRayCaster2D();
    CRayCaster2D(CRayCaster2D& _Origin);
    ~CRayCaster2D();
   
    CLONE(CRayCaster2D)
};

