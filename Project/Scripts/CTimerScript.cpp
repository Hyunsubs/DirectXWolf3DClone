#include "pch.h"
#include "CTimerScript.h"


CTimerScript::CTimerScript()
	: CScript(SCRIPT_TYPE::TIMERSCRIPT)
	, last_time(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "LastTime: ", &last_time);
}

CTimerScript::CTimerScript(CTimerScript& _Origin)
	: CScript(SCRIPT_TYPE::TIMERSCRIPT)
	, last_time(_Origin.last_time)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "LastTime: ", &last_time);
}

CTimerScript::~CTimerScript()
{
}

void CTimerScript::Tick()
{
	last_time -= DT;
	if (last_time <= 0.f)
	{
		last_time = 0.f;
		DeleteObject(GetOwner());
	}
}

void CTimerScript::SaveToFile(FILE* _File)
{
	fwrite(&last_time, sizeof(float), 1, _File);
}

void CTimerScript::LoadFromFile(FILE* _File)
{
	fread(&last_time, sizeof(float), 1, _File);
}


