#pragma once
#include <Engine/CScript.h>

class CRayCastRendererScript :
    public CScript
{
private:
    

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


public:
    CLONE(CRayCastRendererScript)
    CRayCastRendererScript();
    ~CRayCastRendererScript();
};

