#pragma once
#include <Engine/CScript.h>

class CCameraSideWalkScript
	: public CScript
{
private:
	float m_StartTime;
	float m_SideSpeed;
	bool m_Trigger;
	int m_MoveValue;
	Ptr<CSound> menu_sound = FINDSOUND(L"sound\\bgm\\StartScreen.mp3");

public:
	virtual void Begin() override;
	virtual void Tick() override;
	Ptr<CSound> GetMenuSound() { return menu_sound; }

	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);


public:
	CLONE(CCameraSideWalkScript)
	CCameraSideWalkScript();
	CCameraSideWalkScript(CCameraSideWalkScript& _Origin);
	~CCameraSideWalkScript();

};

