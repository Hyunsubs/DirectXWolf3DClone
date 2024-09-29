#pragma once
#include <Engine/CScript.h>


class CSpriteScript :
    public CScript
{
private:
    float m_offsetY;
    Ptr<CTexture> m_TargetTexture;
    bool m_IsCollision;
    bool m_IsFlipBook;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


public:
    CLONE(CSpriteScript)
    CSpriteScript();
    CSpriteScript(CSpriteScript& _Origin);
    ~CSpriteScript();
};

