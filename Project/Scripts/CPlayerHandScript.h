#pragma once
#include <Engine/CScript.h>

enum class WeaponState
{
    KNIFE,
    PISTOL,
    RIFFLE,
    MACHINEGUN,
};

enum class AttackState
{
    FIRE,
    IDLE,
};

class CPlayerHandScript :
    public CScript
{
private:
    WeaponState current_weapon;
    AttackState current_state;
    Ptr<CPrefab> bullet;
    float stiff_time;
    bool is_attack; 
    bool is_minimap;

    Ptr<CSound> knife_sound = FINDSOUND(L"sound\\effect\\Knife.wav");
    Ptr<CSound> pistol_sound = FINDSOUND(L"sound\\effect\\Pistol.wav");
    Ptr<CSound> riffle_sound = FINDSOUND(L"sound\\effect\\Gatling Gun.wav");
    Ptr<CSound> machinegun_sound = FINDSOUND(L"sound\\effect\\Machine Gun.wav");

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void MiniMapCheck();
    void StateCheck();
    void SwitchWeapon();
    void IdleAnim();
    void BeginAttack();
    void MakeBullet();
    
    WeaponState GetCurrentWeaponState() { return current_weapon; }


public:
    CLONE(CPlayerHandScript);
    CPlayerHandScript();
    CPlayerHandScript(CPlayerHandScript& _Origin);
    ~CPlayerHandScript();
};

