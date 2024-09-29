#include "pch.h"
#include "CFadeScript.h"

CFadeScript::CFadeScript()
	: CScript(SCRIPT_TYPE::FADESCRIPT)
	, m_IsIn(false)
	, m_FadeSpeed(0.f)
	, m_FadeFactor(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "FadeSpeed", &m_FadeSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "0 : FadeIn 1: FadeOut", &m_IsIn);
}

CFadeScript::~CFadeScript()
{

}


void CFadeScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
	if (0 == m_IsIn)
		m_FadeFactor = 0.f;
	else
		m_FadeFactor = 1.f;
}

void CFadeScript::Tick()
{
	if (0 == m_IsIn)
		m_FadeFactor += m_FadeSpeed * DT;
	else
		m_FadeFactor -= m_FadeSpeed * DT;
	
	m_FadeFactor = std::clamp(m_FadeFactor, 0.f, 1.f);
	GetRenderComponent()->GetMaterial()->SetScalarParam(FLOAT_0, m_FadeFactor);
}

void CFadeScript::SaveToFile(FILE* _File)
{
	fwrite(&m_FadeSpeed, sizeof(float), 1, _File);
	fwrite(&m_IsIn, sizeof(int), 1, _File);
}

void CFadeScript::LoadFromFile(FILE* _File)
{
	fread(&m_FadeSpeed, sizeof(float), 1, _File);
	fread(&m_IsIn, sizeof(int), 1, _File);
}

