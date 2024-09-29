#include "pch.h"
#include "CPlayerStatusScript.h"
#include "CPlayerHandScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CFontMgr.h>
#include <Engine/define.h>


CPlayerStatusScript::CPlayerStatusScript()
	: CScript(SCRIPT_TYPE::PLAYERSTATUSSCRIPT)
	, pistol_ammo(0)
	, riffle_ammo(0)
	, machinegun_ammo(0)
	, current_weapon(WeaponState::PISTOL)
	, life(100)
	, score(0)
{
}

CPlayerStatusScript::~CPlayerStatusScript()
{
}


void CPlayerStatusScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
	FlipBookComponent()->AddFlipBook(0, FINDFLIPBOOK(L"Animation\\Status\\DownScreen.flip"));
}

void CPlayerStatusScript::Tick()
{
	if (life >= 100)
		life = 100;

	static wchar_t sz_cur_stage[5];
	static wchar_t sz_scoreBuff[50];
	static wchar_t sz_lifeBuff[50];
	static wchar_t sz_bulletBuff[50];
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	Vec3 CamPos = pCurLevel->FindObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	CGameObject* pRayCaster = pCurLevel->FindObjectByName(L"RayCaster2D");
	CGameObject* pPlayerHand = pCurLevel->FindObjectByName(L"PlayerHand");

	extern int g_iTargetLevel;

	if(pRayCaster && !(pRayCaster->RayCaster2D()->IsMiniMap() && pPlayerHand))
	{
		swprintf_s(sz_cur_stage, 5, L"%d", g_iTargetLevel);
		level_font = { sz_cur_stage, 50.f, 630.f, 60.f, (UINT)FONT_RGBA(255, 255, 255, 255) };
		Transform()->SetRelativePos(Vec3(CamPos.x, CamPos.y - 300.f, 100.f));
		Transform()->SetRelativeScale(Vec3(965.f, 120.f, 0.f));
		swprintf_s(sz_scoreBuff, 50, L"%d", score);
		score_font = { sz_scoreBuff, 200.f, 630.f, 60.f, (UINT)FONT_RGBA(255, 255, 255, 255)};
		swprintf_s(sz_lifeBuff, 50, L"%d", life);
		life_font = { sz_lifeBuff, 520.f, 630.f, 60.f, (UINT)FONT_RGBA(255, 255, 255, 255)};
		CFontMgr::GetInst()->AddFont(&score_font);
		CFontMgr::GetInst()->AddFont(&life_font);
		CFontMgr::GetInst()->AddFont(&level_font);
		FlipBookComponent()->Play(0, 1.f, true);

		vector<CScript*> vec_scripts = pPlayerHand->GetScripts();
		CPlayerHandScript* pHandScript = nullptr;
		for (int i = 0; i < vec_scripts.size(); i++)
		{
			if (vec_scripts[i]->GetScriptType() == SCRIPT_TYPE::PLAYERHANDSCRIPT)
			{
				pHandScript = (CPlayerHandScript*)vec_scripts[i];
			}
		}

		if (pHandScript)
		{
			WeaponState curWeapon = pHandScript->GetCurrentWeaponState();

			switch (curWeapon)
			{
			case WeaponState::KNIFE:
				swprintf_s(sz_bulletBuff, 50, L"¡Ä");
				break;
			case WeaponState::PISTOL:
				swprintf_s(sz_bulletBuff, 50, L"%d", pistol_ammo);
				break;
			case WeaponState::RIFFLE:
				swprintf_s(sz_bulletBuff, 50, L"%d", riffle_ammo);
				break;
			case WeaponState::MACHINEGUN:
				swprintf_s(sz_bulletBuff, 50, L"%d", machinegun_ammo);
				break;
			default:
				break;
			}
			ammo_font = { sz_bulletBuff, 640.f, 640.f, 60.f, (UINT)FONT_RGBA(255, 255, 255, 255) };
			CFontMgr::GetInst()->AddFont(&ammo_font);
		}
	}

	GameOver();
}

void CPlayerStatusScript::AddAmmo(AmmoType _ammotype, int _amount)
{
	switch (_ammotype)
	{
	case AmmoType::Pistol:
		pistol_ammo += _amount;
		break;
	case AmmoType::Riffle:
		riffle_ammo += _amount;
		break;
	case AmmoType::MachineGun:
		machinegun_ammo += _amount;
		break;
	default:
		break;
	}
}

void CPlayerStatusScript::GameOver()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (life <= 0 && !is_dead)
	{
		is_dead = true;
		life = 0;
		CGameObject* pRayCaster = pCurLevel->FindObjectByName(L"RayCaster2D");
		CGameObject* pPlayerHand = pCurLevel->FindObjectByName(L"PlayerHand");

		DeleteObject(pRayCaster);
		DeleteObject(pPlayerHand);
		Ptr<CPrefab> noiseRed = FINDPREFAB(L"prefab\\NoiseRed.pref");
		Instantiate(noiseRed, 0, Vec3(), noiseRed->GetKey());
		gameover_sound->Play(1, 1, false);
	}

	if (is_dead)
	{
		dead_acc_time += DT;
	}

	if (dead_acc_time >= 2.f)
	{
		extern int g_iTargetLevel;
		extern bool g_bLevelTrigger;

		g_iTargetLevel = 4;
		g_bLevelTrigger = true;
	}
}


void CPlayerStatusScript::SaveToFile(FILE* _File)
{
}

void CPlayerStatusScript::LoadFromFile(FILE* _File)
{
}


