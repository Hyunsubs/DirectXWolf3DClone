#include "pch.h"
#include "CAlphaNoiseScript.h"

CAlphaNoiseScript::CAlphaNoiseScript()
	: CScript(SCRIPT_TYPE::ALPHANOISESCRIPT)
	, m_AlphaAllowance(1.f)
	, m_Speed(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "NoiseSpeed", &m_Speed);
}

CAlphaNoiseScript::~CAlphaNoiseScript()
{
}


void CAlphaNoiseScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
}

void CAlphaNoiseScript::Tick()
{
	m_AlphaAllowance -= m_Speed * EngineDT;
	if (m_AlphaAllowance <= 0.f)
	{
		m_AlphaAllowance = 0.f;
	}
	GetRenderComponent()->GetMaterial()->SetScalarParam(FLOAT_0, m_AlphaAllowance);
}

void CAlphaNoiseScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CAlphaNoiseScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}

