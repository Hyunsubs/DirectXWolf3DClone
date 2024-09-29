#pragma once
#include <Engine/CScript.h>
class CDelayedInstantiator
	: public CScript
{
private:
	float m_DelayedTime;
	bool m_SyncTrigger;
	Ptr<CPrefab> m_TargetPref;

public:
	virtual void Tick() override;


	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;



public:
	CLONE(CDelayedInstantiator)
	CDelayedInstantiator();
	~CDelayedInstantiator();
};

