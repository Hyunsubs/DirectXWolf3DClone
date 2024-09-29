#include "pch.h"
#include "CLevelChangerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CAssetMgr.h>

#include "CCameraSideWalkScript.h"

CLevelChangerScript::CLevelChangerScript()
	: CScript(SCRIPT_TYPE::LEVELCHANGERSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::INT, "TargetLevel", &m_TargetLevel);
}

CLevelChangerScript::CLevelChangerScript(CLevelChangerScript& _Origin)
	: CScript(SCRIPT_TYPE::LEVELCHANGERSCRIPT)
	, m_TargetLevel(_Origin.m_TargetLevel)
{
	AddScriptParam(SCRIPT_PARAM::INT, "TargetLevel", &m_TargetLevel);
}

CLevelChangerScript::~CLevelChangerScript()
{
}


void CLevelChangerScript::Tick()
{
	extern int g_iTargetLevel;
	extern bool g_bLevelTrigger;

	if (m_Trigger)
	{
		m_Trigger = false;
		g_bLevelTrigger = true;
		g_iTargetLevel = m_TargetLevel;
	}

	if (KEY_TAP(KEY::P))
	{
		CGameObject* pTargetMenu = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
		CCameraSideWalkScript* script = nullptr;
		if (pTargetMenu)
		{
			vector<CScript*> vec_scripts = pTargetMenu->GetScripts();
			for (int i = 0; i < vec_scripts.size(); i++)
			{
				if (SCRIPT_TYPE::CAMERASIDEWALKSCRIPT == vec_scripts[i]->GetScriptType())
					script = (CCameraSideWalkScript*)vec_scripts[i];
			}
			if (script)
			{
				Ptr<CSound> menu_sound = script->GetMenuSound();
				menu_sound->Stop();
			}
		}

		m_Trigger = true;
		Ptr<CSound> select_sound = FINDSOUND(L"sound\\effect\\Menu Select.wav");
		select_sound->Play(1, 1, false);
	}
}

void CLevelChangerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_TargetLevel, sizeof(int), 1, _File);
}

void CLevelChangerScript::LoadFromFile(FILE* _File)
{
	fread(&m_TargetLevel, sizeof(int), 1, _File);
}

