#include "pch.h"
#include "CInterMissionScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CPlayerStatusScript.h"
#include "CLevelChangerScript.h"

CInterMissionScript::CInterMissionScript()
	: CScript(SCRIPT_TYPE::INTERMISSIONSCRIPT)
	, change_level(false)
	, down_finished(false)
{
}

CInterMissionScript::CInterMissionScript(CInterMissionScript& _Origin)
	: CScript(SCRIPT_TYPE::INTERMISSIONSCRIPT)
	, change_level(false)
	, down_finished(false)
{
}

CInterMissionScript::~CInterMissionScript()
{
}

void CInterMissionScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
}

void CInterMissionScript::Tick()
{
	extern int g_iTargetLevel;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* pTargetCam = pCurLevel->FindObjectByName(L"MainCamera");
	Vec3 CurPos = Transform()->GetRelativePos();
	static wchar_t sz_score[50] = {};

	if (pTargetCam)
	{
		if (CurPos.y > pTargetCam->Transform()->GetWorldPos().y)
		{
			CurPos.y -= DT * 400.f;
		}
		else
		{
			down_finished = true;
		}
	}


	Ptr<CPrefab> level_changer;
	if (down_finished && !change_level)
	{
		switch (g_iTargetLevel)
		{
		case 0:
			level_changer = FINDPREFAB(L"prefab\\LevelChanger1.pref");
			break;
		case 1:
			level_changer = FINDPREFAB(L"prefab\\LevelChanger2.pref");
			break;
		case 2:
			level_changer = FINDPREFAB(L"prefab\\LevelChanger3.pref");
			break;
		case 3:
			level_changer = FINDPREFAB(L"prefab\\LevelChanger4.pref");
			break;
		case 4:
			level_changer = FINDPREFAB(L"prefab\\LevelChanger0.pref");
			break;
		case 5:
			level_changer = FINDPREFAB(L"prefab\\LevelChanger0.pref");
			break;
		default:
			break;
		}
		Instantiate(level_changer, 0, Vec3(), level_changer->GetKey());
		Ptr<CPrefab> pref = FINDPREFAB(L"prefab\\StageClear!.pref");
		Instantiate(pref, (UINT)LAYER_NAME::UI, Vec3(), L"STAGE CLEAR!");
		change_level = true;
	}


	Transform()->SetRelativePos(CurPos);

}

void CInterMissionScript::SaveToFile(FILE* _File)
{
}

void CInterMissionScript::LoadFromFile(FILE* _File)
{
}




