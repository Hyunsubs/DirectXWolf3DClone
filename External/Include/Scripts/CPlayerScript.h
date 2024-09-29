#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
private:
    float           m_Speed;

    CGameObject*    m_pTileMap;
    bool            m_bFoundTileMap;
    bool            m_bCanMove;
    bool            m_bInstantiated;


    int tile_col;
    int tile_row;
    Vec2 tile_size;

    bool screen_instantiate;
    bool melt_start;
    float acc_time;

    Ptr<CSound> clear_sound = FINDSOUND(L"sound\\effect\\Yeeeah!.wav");
    Ptr<CSound> dying_sound = FINDSOUND(L"sound\\effect\\Player Dies.wav");
    Ptr<CSound> attacked_sound = FINDSOUND(L"sound\\effect\\Player Pain 1.wav");
    Ptr<CSound> door_opening_sound = FINDSOUND(L"sound\\effect\\Door.wav");


public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    CPlayerScript(CPlayerScript& _Origin);
    ~CPlayerScript();
};

