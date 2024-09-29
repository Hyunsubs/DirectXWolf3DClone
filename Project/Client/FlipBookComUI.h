#pragma once
#include "ComponentUI.h"
class FlipBookComUI :
    public ComponentUI
{
private:
    virtual void Update() override;

    void SelectFlip(DWORD_PTR _ListUI);
public:
    FlipBookComUI();
    ~FlipBookComUI();

};

