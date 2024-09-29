#include "pch.h"
#include "CUiAnimScript.h"

CUiAnimScript::CUiAnimScript()
	: CScript(SCRIPT_TYPE::UIANIMSCRIPT)
	, m_Fps(0)
	, m_AccTime(1.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "playtime", &m_Fps);
}

CUiAnimScript::~CUiAnimScript()
{
}

void CUiAnimScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
	FlipBookComponent()->Play(0, m_Fps, false);
}

void CUiAnimScript::Tick()
{
	m_AccTime -= DT;
	if (m_AccTime <= 0.f)
	{
		m_AccTime = 1.f;
		FlipBookComponent()->Play(0, m_Fps, false);
	}
}

void CUiAnimScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Fps, sizeof(int), 1, _File);
}

void CUiAnimScript::LoadFromFile(FILE* _File)
{
	fread(&m_Fps, sizeof(int), 1, _File);
}


