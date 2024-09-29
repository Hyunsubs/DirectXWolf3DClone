#pragma once
#include <Engine/CScript.h>

class CFadeScript 
	: public CScript
{
private:
	bool m_IsIn;
	float m_FadeSpeed;
	float m_FadeFactor;

public:
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE(CFadeScript)
	CFadeScript();
	~CFadeScript();
};

