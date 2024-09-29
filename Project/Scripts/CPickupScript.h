#pragma once
#include <Engine/CScript.h>

enum class PickupType
{
    Heal,
    Score,
    Ammo,
};

enum class AmmoType
{
    None,
    Pistol,
    Riffle,
    MachineGun,
};


class CPickupScript :
    public CScript
{
private:
    PickupType m_PickupType;
    AmmoType m_AmmoType;
    int m_Amount;

    Ptr<CSound> heal_sound = FINDSOUND(L"sound\\effect\\Health.wav");
    Ptr<CSound> score_sound = FINDSOUND(L"sound\\effect\\Pickup.wav");
    Ptr<CSound> ammo_sound = FINDSOUND(L"sound\\effect\\Ammo.wav");

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


public:
    CLONE(CPickupScript)
    CPickupScript();
    CPickupScript(CPickupScript& _Origin);
    ~CPickupScript();
};

