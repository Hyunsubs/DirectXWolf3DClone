#pragma once
#include "EditorUI.h"

class Outliner :
    public EditorUI
{
private:
    class TreeUI*   m_Tree;
    class CGameObject* m_deleteObj;
public:
    virtual void Update() override;

private:
    void RenewLevel();
    void GameObjectClicked(DWORD_PTR _Param);
    void AddGameObject(class TreeNode* pNode, class CGameObject* _Object);

    void GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2);
    void DroppedFromOuter(DWORD_PTR _OuterData, DWORD_PTR _DropNode);  

    void PopupMenu(DWORD_PTR _Param);


public:
    Outliner();
    ~Outliner();
};

