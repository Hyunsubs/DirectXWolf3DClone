#pragma once
#include <Engine/CScript.h>
class CLevelChangerScript
	: public CScript
{
private:
	virtual void Tick() override;


	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	int m_TargetLevel;
	bool m_Trigger;

public:
	void TurnTrigger() { m_Trigger = true; }
	void SetTargetLevel(int _Level) { m_TargetLevel = _Level; }


public:
	CLONE(CLevelChangerScript)
	CLevelChangerScript();
	CLevelChangerScript(CLevelChangerScript& _Origin);
	~CLevelChangerScript();
};

