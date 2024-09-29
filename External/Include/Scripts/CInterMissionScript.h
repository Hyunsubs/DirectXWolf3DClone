#pragma once
#include <Engine/CScript.h>

class CLevelChangerScript;

class CInterMissionScript
	: public CScript
{
private:
	bool down_finished;
	bool change_level;
	bool found_script;
	int score;

	CLevelChangerScript* targetScript;
	FontData score_font;


public:
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE(CInterMissionScript)
	CInterMissionScript();
	CInterMissionScript(CInterMissionScript& _Origin);
	~CInterMissionScript();

};

