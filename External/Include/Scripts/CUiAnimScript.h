#pragma once
#include <Engine/CScript.h>


// ���ӿ��� ��ũ�� ������
// �����ð����� 0�� �ִϸ��̼��� �������ָ� �ȴ�.
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

