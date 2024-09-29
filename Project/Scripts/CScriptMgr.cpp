#include "pch.h"
#include "CScriptMgr.h"

#include "CAlphaNoiseScript.h"
#include "CBulletScript.h"
#include "CCameraMoveScript.h"
#include "CCameraSideWalkScript.h"
#include "CDelayedInstantiator.h"
#include "CFadeScript.h"
#include "CFlashFont.h"
#include "CFontPrintScript.h"
#include "CInterMissionScript.h"
#include "CLevelChangerScript.h"
#include "CMonsterScript.h"
#include "CPickupScript.h"
#include "CPlayerHandScript.h"
#include "CPlayerScript.h"
#include "CPlayerSelectWeapon.h"
#include "CPlayerStatusScript.h"
#include "CRayCastRendererScript.h"
#include "CSpriteScript.h"
#include "CTimerScript.h"
#include "CUiAnimScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAlphaNoiseScript");
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCameraSideWalkScript");
	_vec.push_back(L"CDelayedInstantiator");
	_vec.push_back(L"CFadeScript");
	_vec.push_back(L"CFlashFont");
	_vec.push_back(L"CFontPrintScript");
	_vec.push_back(L"CInterMissionScript");
	_vec.push_back(L"CLevelChangerScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPickupScript");
	_vec.push_back(L"CPlayerHandScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerSelectWeapon");
	_vec.push_back(L"CPlayerStatusScript");
	_vec.push_back(L"CRayCastRendererScript");
	_vec.push_back(L"CSpriteScript");
	_vec.push_back(L"CTimerScript");
	_vec.push_back(L"CUiAnimScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAlphaNoiseScript" == _strScriptName)
		return new CAlphaNoiseScript;
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCameraSideWalkScript" == _strScriptName)
		return new CCameraSideWalkScript;
	if (L"CDelayedInstantiator" == _strScriptName)
		return new CDelayedInstantiator;
	if (L"CFadeScript" == _strScriptName)
		return new CFadeScript;
	if (L"CFlashFont" == _strScriptName)
		return new CFlashFont;
	if (L"CFontPrintScript" == _strScriptName)
		return new CFontPrintScript;
	if (L"CInterMissionScript" == _strScriptName)
		return new CInterMissionScript;
	if (L"CLevelChangerScript" == _strScriptName)
		return new CLevelChangerScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPickupScript" == _strScriptName)
		return new CPickupScript;
	if (L"CPlayerHandScript" == _strScriptName)
		return new CPlayerHandScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerSelectWeapon" == _strScriptName)
		return new CPlayerSelectWeapon;
	if (L"CPlayerStatusScript" == _strScriptName)
		return new CPlayerStatusScript;
	if (L"CRayCastRendererScript" == _strScriptName)
		return new CRayCastRendererScript;
	if (L"CSpriteScript" == _strScriptName)
		return new CSpriteScript;
	if (L"CTimerScript" == _strScriptName)
		return new CTimerScript;
	if (L"CUiAnimScript" == _strScriptName)
		return new CUiAnimScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ALPHANOISESCRIPT:
		return new CAlphaNoiseScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASIDEWALKSCRIPT:
		return new CCameraSideWalkScript;
		break;
	case (UINT)SCRIPT_TYPE::DELAYEDINSTANTIATOR:
		return new CDelayedInstantiator;
		break;
	case (UINT)SCRIPT_TYPE::FADESCRIPT:
		return new CFadeScript;
		break;
	case (UINT)SCRIPT_TYPE::FLASHFONT:
		return new CFlashFont;
		break;
	case (UINT)SCRIPT_TYPE::FONTPRINTSCRIPT:
		return new CFontPrintScript;
		break;
	case (UINT)SCRIPT_TYPE::INTERMISSIONSCRIPT:
		return new CInterMissionScript;
		break;
	case (UINT)SCRIPT_TYPE::LEVELCHANGERSCRIPT:
		return new CLevelChangerScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PICKUPSCRIPT:
		return new CPickupScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHANDSCRIPT:
		return new CPlayerHandScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSELECTWEAPON:
		return new CPlayerSelectWeapon;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSTATUSSCRIPT:
		return new CPlayerStatusScript;
		break;
	case (UINT)SCRIPT_TYPE::RAYCASTRENDERERSCRIPT:
		return new CRayCastRendererScript;
		break;
	case (UINT)SCRIPT_TYPE::SPRITESCRIPT:
		return new CSpriteScript;
		break;
	case (UINT)SCRIPT_TYPE::TIMERSCRIPT:
		return new CTimerScript;
		break;
	case (UINT)SCRIPT_TYPE::UIANIMSCRIPT:
		return new CUiAnimScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ALPHANOISESCRIPT:
		return L"CAlphaNoiseScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CAMERASIDEWALKSCRIPT:
		return L"CCameraSideWalkScript";
		break;

	case SCRIPT_TYPE::DELAYEDINSTANTIATOR:
		return L"CDelayedInstantiator";
		break;

	case SCRIPT_TYPE::FADESCRIPT:
		return L"CFadeScript";
		break;

	case SCRIPT_TYPE::FLASHFONT:
		return L"CFlashFont";
		break;

	case SCRIPT_TYPE::FONTPRINTSCRIPT:
		return L"CFontPrintScript";
		break;

	case SCRIPT_TYPE::INTERMISSIONSCRIPT:
		return L"CInterMissionScript";
		break;

	case SCRIPT_TYPE::LEVELCHANGERSCRIPT:
		return L"CLevelChangerScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PICKUPSCRIPT:
		return L"CPickupScript";
		break;

	case SCRIPT_TYPE::PLAYERHANDSCRIPT:
		return L"CPlayerHandScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERSELECTWEAPON:
		return L"CPlayerSelectWeapon";
		break;

	case SCRIPT_TYPE::PLAYERSTATUSSCRIPT:
		return L"CPlayerStatusScript";
		break;

	case SCRIPT_TYPE::RAYCASTRENDERERSCRIPT:
		return L"CRayCastRendererScript";
		break;

	case SCRIPT_TYPE::SPRITESCRIPT:
		return L"CSpriteScript";
		break;

	case SCRIPT_TYPE::TIMERSCRIPT:
		return L"CTimerScript";
		break;

	case SCRIPT_TYPE::UIANIMSCRIPT:
		return L"CUiAnimScript";
		break;

	}
	return nullptr;
}