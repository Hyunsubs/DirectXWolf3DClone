#pragma once
#include <Engine\CScript.h>

class CPlayerSelectWeapon :
    public CScript
{

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CPlayerSelectWeapon)
    CPlayerSelectWeapon();
    ~CPlayerSelectWeapon();

};

