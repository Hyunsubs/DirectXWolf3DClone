#pragma once
#include <Engine/CScript.h>

enum class MonsterState
{
    WALK,
    ATTACK,
    DEAD,
};


class CMonsterScript :
    public CScript
{

private:
    MonsterState m_CurState;
    CGameObject* m_TargetObject;
    Ptr<CPrefab> m_BulletPrefab;

    int m_Hp;
    int m_BulletLayer;
    int m_AttackTime;
    int m_BulletAmmo;

    bool m_IsAnim;
    bool m_IsDead;
    bool m_HitTrigger;

    float m_Speed;
    float m_AnimTime;
    float m_AnimSpeed;
    float m_AnimTimer;
    float m_BulletTimer;
    float m_BulletIntermission;

    float m_dead_acc_time = 0.f;
    bool is_deathcam = false;

    float sound_time = 20.f;

    Ptr<CSound> attacked_sound = FINDSOUND(L"sound\\effect\\Enemy Pain.wav");
    Ptr<CSound> attacking_sound = FINDSOUND(L"sound\\effect\\Pistol.wav");
    Ptr<CSound> dead_sound = FINDSOUND(L"sound\\effect\\Death 1.wav");
    Ptr<CSound> monster_sound0 = FINDSOUND(L"sound\\effect\\Achtung!.wav");
    Ptr<CSound> monster_sound1 = FINDSOUND(L"sound\\effect\\Boss Speak 1.wav");
    Ptr<CSound> monster_sound2 = FINDSOUND(L"sound\\effect\\Boss Speak 2.wav");
    Ptr<CSound> monster_sound3 = FINDSOUND(L"sound\\effect\\Halt 2.wav");
    Ptr<CSound> monster_sound4 = FINDSOUND(L"sound\\effect\\Halt.wav");
    Ptr<CSound> monster_sound5 = FINDSOUND(L"sound\\effect\\Guten Tag!.wav");
    Ptr<CSound> monster_sound6 = FINDSOUND(L"sound\\effect\\Halten Sie!.wav");
    Ptr<CSound> monster_sound7 = FINDSOUND(L"sound\\effect\\Mechahitler Hoof.wav");
    

public:
    virtual void Tick() override;

    void FollowPlayer();
    void WalkAnim();

    void AttackFunc();
    void AttackAnim();

    void DeadAnim();


    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;



public:
    CLONE(CMonsterScript)
    CMonsterScript();
    CMonsterScript(CMonsterScript& _Origin);
    ~CMonsterScript();
};

