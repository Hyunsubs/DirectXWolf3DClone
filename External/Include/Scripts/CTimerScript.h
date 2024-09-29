#pragma once
#include <Engine/CScript.h>
class CTimerScript
	: public CScript
{
private:
	float last_time;

public:
	virtual void Tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE(CTimerScript)
	CTimerScript();
	CTimerScript(CTimerScript& _Origin);
	~CTimerScript();
};

