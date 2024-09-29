#include "pch.h"
#include "CCameraSideWalkScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CCameraSideWalkScript::CCameraSideWalkScript()
	: CScript(SCRIPT_TYPE::CAMERASIDEWALKSCRIPT)
	, m_StartTime(0.f)
	, m_SideSpeed(0.f)
	, m_MoveValue(0)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "StartTime", &m_StartTime);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SideSpeed", &m_SideSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "MoveValue", &m_MoveValue);
}

CCameraSideWalkScript::CCameraSideWalkScript(CCameraSideWalkScript& _Origin)
	: CScript(SCRIPT_TYPE::CAMERASIDEWALKSCRIPT)
	, m_StartTime(_Origin.m_StartTime)
	, m_SideSpeed(_Origin.m_SideSpeed)
	, m_MoveValue(_Origin.m_MoveValue)
{
}

CCameraSideWalkScript::~CCameraSideWalkScript()
{
}


void CCameraSideWalkScript::Begin()
{
	menu_sound->Play(1, 1, false);
}

void CCameraSideWalkScript::Tick()
{
	m_StartTime -= DT;
	Vec3 CurPos = Transform()->GetRelativePos();
	if (m_StartTime <= 0.f && CurPos.x <= m_MoveValue)
	{
		m_StartTime = 0.f;
		CurPos.x += m_SideSpeed * DT;
		Transform()->SetRelativePos(CurPos);
	}

	if (CurPos.x >= m_MoveValue && !m_Trigger)
	{
		Ptr<CPrefab> levelChanger = FINDPREFAB(L"prefab\\LevelChanger1.pref");
		Instantiate(levelChanger, 0, Vec3(), levelChanger->GetKey());
		m_Trigger = true;
	}
}



void CCameraSideWalkScript::SaveToFile(FILE* _File)
{
	fwrite(&m_StartTime, sizeof(float), 1, _File);
	fwrite(&m_SideSpeed ,sizeof(float), 1, _File);
	fwrite(&m_MoveValue, sizeof(int), 1, _File);
}

void CCameraSideWalkScript::LoadFromFile(FILE* _File)
{
	fread(&m_StartTime, sizeof(float), 1, _File);
	fread(&m_SideSpeed, sizeof(float), 1, _File);
	fread(&m_MoveValue, sizeof(int), 1, _File);
}
