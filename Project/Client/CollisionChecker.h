#pragma once
#include "EditorUI.h"
class CollisionChecker :
    public EditorUI
{
private:
	vector<Vec2> m_CollisionList;
	static Vec2 m_CollisionInput;


public:
	virtual void Update() override;
	void AddCollisionCheck();
	void ClearCollision();
	void ShowCollisionList();

public:
	CollisionChecker();
	~CollisionChecker();
};

