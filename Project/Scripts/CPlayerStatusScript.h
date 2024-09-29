#pragma once
#include <Engine/CScript.h>
#include <Engine/CFontMgr.h>
#include "CPlayerHandScript.h"
#include "CPickupScript.h"


class CPlayerStatusScript :
    public CScript
{
public:
    int score;
    int pistol_ammo;
    int riffle_ammo;
    int machinegun_ammo;
    
    WeaponState current_weapon;
    int life;

    FontData level_font;
    FontData score_font;
    FontData life_font;
    FontData ammo_font;

    Ptr<CSound> gameover_sound = FINDSOUND(L"sound\\effect\\Player Pain 2.wav");
    bool is_dead;
    float dead_acc_time;

public:
    virtual void Begin() override;
    virtual void Tick() override;


    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void AddHp(int _amount) { life += _amount; }
    void AddScore(int _amount) { score += _amount; }
    void AddAmmo(AmmoType _ammotype, int _amount);

    int GetScore() { return score; }

    int GetAmmo(WeaponState _ammotype)
    {
        switch (_ammotype)
        {
        case WeaponState::PISTOL:
            return pistol_ammo;
            break;
        case WeaponState::RIFFLE:
            return riffle_ammo;
            break;
        case WeaponState::MACHINEGUN:
            return machinegun_ammo;
            break;
        default:
            return 0;
            break;
        }
    }

    void GameOver();


public:
    CPlayerStatusScript();
    ~CPlayerStatusScript();
    CLONE(CPlayerStatusScript)
};

