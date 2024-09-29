#include "pch.h"
#include "CPickupScript.h"
#include "CPlayerStatusScript.h"
#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>

CPickupScript::CPickupScript()
	: CScript(SCRIPT_TYPE::PICKUPSCRIPT)
	, m_Amount(0)
	, m_PickupType(PickupType::Heal)
	, m_AmmoType(AmmoType::Pistol)
{
	AddScriptParam(SCRIPT_PARAM::INT, "PickUpType 0 :Heal 1:Score 2:Ammo", &m_PickupType);
	AddScriptParam(SCRIPT_PARAM::INT, "AmmoType 0:Pistol, 1:Riffle, 2:Machinegun", &m_AmmoType);
	AddScriptParam(SCRIPT_PARAM::INT, "Amount", &m_Amount);
}

CPickupScript::CPickupScript(CPickupScript& _Origin)
	: CScript(SCRIPT_TYPE::PICKUPSCRIPT)
	, m_Amount(_Origin.m_Amount)
	, m_PickupType(_Origin.m_PickupType)
	, m_AmmoType(_Origin.m_AmmoType)
{
	AddScriptParam(SCRIPT_PARAM::INT, "PickUpType 0 :Heal 1:Score 2:Ammo", &m_PickupType);
	AddScriptParam(SCRIPT_PARAM::INT, "AmmoType 0:Pistol, 1:Riffle, 2:Machinegun", &m_AmmoType);
	AddScriptParam(SCRIPT_PARAM::INT, "Amount", &m_Amount);
}

CPickupScript::~CPickupScript()
{
}


void CPickupScript::Begin()
{
}

void CPickupScript::Tick()
{
}

void CPickupScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (_OtherObject == pCurLevel->FindObjectByName(L"RayCaster2D"))
	{
		CGameObject* statusObj = pCurLevel->FindObjectByName(L"ScoreBoard");
		if (statusObj)
		{
			vector<CScript*> scripts = statusObj->GetScripts();
			CPlayerStatusScript* status = nullptr;

			for (int i = 0; i < scripts.size(); i++)
			{
				if (SCRIPT_TYPE::PLAYERSTATUSSCRIPT == scripts[i]->GetScriptType())
					status = (CPlayerStatusScript*)scripts[i];
			}

			if (status)
			{
				switch (m_PickupType)
				{
				case PickupType::Heal:
					status->AddHp(m_Amount);
					heal_sound->Play(1, 1, true);
					break;
				case PickupType::Score:
					status->AddScore(m_Amount);
					score_sound->Play(1, 1, true);
					break;
				case PickupType::Ammo:
					status->AddAmmo(AmmoType((UINT)m_AmmoType+1), m_Amount);
					ammo_sound->Play(1, 1, true);
					break;
				default:
					break;
				}
			}

			Ptr<CPrefab> yellowScreen = FINDPREFAB(L"prefab\\YellowScreen.pref");
			Instantiate(yellowScreen, 0, Vec3(), L"YellowScreen");

			DeleteObject(GetOwner());
		}

	}


}

void CPickupScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Amount, sizeof(int), 1, _File);
	fwrite(&m_PickupType, sizeof(PickupType), 1, _File);
	fwrite(&m_AmmoType, sizeof(AmmoType), 1, _File);
}

void CPickupScript::LoadFromFile(FILE* _File)
{
	fread(&m_Amount, sizeof(int), 1, _File);
	fread(&m_PickupType, sizeof(PickupType), 1, _File);
	fread(&m_AmmoType, sizeof(AmmoType), 1, _File);
}



