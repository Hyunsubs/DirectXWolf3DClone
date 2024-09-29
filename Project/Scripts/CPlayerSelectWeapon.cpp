#include "pch.h"
#include "CPlayerSelectWeapon.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CPlayerHandScript.h"

CPlayerSelectWeapon::CPlayerSelectWeapon()
	: CScript(SCRIPT_TYPE::PLAYERSELECTWEAPON)
{
}

CPlayerSelectWeapon::~CPlayerSelectWeapon()
{
}

void CPlayerSelectWeapon::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerSelectWeapon::Tick()
{
	CGameObject* pTargetCam = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"MainCamera");
	if (pTargetCam)
	{
		Vec3 CamPos = pTargetCam->Transform()->GetRelativePos();
		Transform()->SetRelativePos(Vec3(CamPos.x + 360.f, CamPos.y - 300.f, 100.f));
		Transform()->SetRelativeScale(Vec3(180.f, 100.f, 1.f));
	}



	CGameObject* pTargetHand = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerHand");
	CPlayerHandScript* script = nullptr;
	if (pTargetHand)
	{
		vector<CScript*> vec_scripts = pTargetHand->GetScripts();
		for (int i = 0; i < vec_scripts.size(); i++)
		{
			if (vec_scripts[i]->GetScriptType() == SCRIPT_TYPE::PLAYERHANDSCRIPT)
				script = (CPlayerHandScript*)vec_scripts[i];
		}
	}

	if (script)
	{
		WeaponState currentWeapon = script->GetCurrentWeaponState();
		switch (currentWeapon)
		{
		case WeaponState::KNIFE:
			FlipBookComponent()->Play(0, 1.f, true);
			break;
		case WeaponState::PISTOL:
			FlipBookComponent()->Play(1, 1.f, true);
			break;
		case WeaponState::RIFFLE:
			FlipBookComponent()->Play(2, 1.f, true);
			break;
		case WeaponState::MACHINEGUN:
			FlipBookComponent()->Play(3, 1.f, true);
			break;
		default:
			break;
		}
	}

}

void CPlayerSelectWeapon::SaveToFile(FILE* _File)
{
}

void CPlayerSelectWeapon::LoadFromFile(FILE* _File)
{
}


