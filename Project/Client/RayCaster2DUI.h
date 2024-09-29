#pragma once
#include "ComponentUI.h"
class RayCaster2DUI :
    public ComponentUI
{

private:
    virtual void Update() override;
    void SetTargetObj(DWORD_PTR _ListUI);

public:
    RayCaster2DUI();
    ~RayCaster2DUI();


};

