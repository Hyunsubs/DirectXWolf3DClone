#pragma once
#include <Engine/CScript.h>


// 게임오버 스크린 구현용
// 일정시간마다 0번 애니메이션을 실행해주면 된다.
class CUiAnimScript
	: public CScript
{
private:
	int m_Fps;
	float m_AccTime;

public:
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;


public:
	CLONE(CUiAnimScript)
	CUiAnimScript();
	~CUiAnimScript();

};

