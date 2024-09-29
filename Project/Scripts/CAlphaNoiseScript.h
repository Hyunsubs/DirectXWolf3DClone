#pragma once
#include <Engine/CScript.h>

class CAlphaNoiseScript
	: public CScript
{
private:
	float m_AlphaAllowance;
	float m_Speed;

public:
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE(CAlphaNoiseScript)
	CAlphaNoiseScript();
	~CAlphaNoiseScript();
};

