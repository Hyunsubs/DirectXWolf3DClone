#include "pch.h"
#include "CPlayerHandScript.h"
#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include "CBulletScript.h"
#include "CPlayerStatusScript.h"

CPlayerHandScript::CPlayerHandScript()
	: CScript(SCRIPT_TYPE::PLAYERHANDSCRIPT)
	, stiff_time(0.f)
	, current_weapon(WeaponState::KNIFE)
	, is_attack(false)
{
}

CPlayerHandScript::CPlayerHandScript(CPlayerHandScript& _Origin)
	: CScript(SCRIPT_TYPE::PLAYERHANDSCRIPT)
	, stiff_time(_Origin.stiff_time)
	, current_weapon(_Origin.current_weapon)
	, is_attack(_Origin.is_attack)
{

}

CPlayerHandScript::~CPlayerHandScript()
{
}

void CPlayerHandScript::Begin()
{
	bullet = FINDPREFAB(L"prefab\\Bullet.pref");
	GetRenderComponent()->GetDynamicMaterial();
	FlipBookComponent()->AddFlipBook(0, FINDFLIPBOOK(L"Animation\\Attack\\KnifeAttack.flip"));
	FlipBookComponent()->AddFlipBook(1, FINDFLIPBOOK(L"Animation\\Attack\\PistolAttack.flip"));
	FlipBookComponent()->AddFlipBook(2, FINDFLIPBOOK(L"Animation\\Attack\\MachineAttack.flip"));
	FlipBookComponent()->AddFlipBook(3, FINDFLIPBOOK(L"Animation\\Attack\\KnifeIdle.flip"));
	FlipBookComponent()->AddFlipBook(4, FINDFLIPBOOK(L"Animation\\Attack\\PistolIdle.flip"));
	FlipBookComponent()->AddFlipBook(5, FINDFLIPBOOK(L"Animation\\Attack\\MachineIdle.flip"));
	FlipBookComponent()->AddFlipBook(6, FINDFLIPBOOK(L"Animation\\Attack\\BFGIdle.flip"));
	FlipBookComponent()->AddFlipBook(7, FINDFLIPBOOK(L"Animation\\Attack\\BFGAttack.flip"));
}

void CPlayerHandScript::Tick()
{
	if(AttackState::FIRE == current_state)
		stiff_time -= DT;

	MiniMapCheck();
	StateCheck();
	IdleAnim();
	SwitchWeapon();

	bool can_shoot = false;
	CGameObject* status = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"ScoreBoard");
	CPlayerStatusScript* target = nullptr;
	if (status)
	{
		vector<CScript*> scripts = status->GetScripts();
		for (int i = 0; i < scripts.size(); i++)
		{
			if (SCRIPT_TYPE::PLAYERSTATUSSCRIPT == scripts[i]->GetScriptType())
				target = (CPlayerStatusScript*)scripts[i];
		}	
	}

	target->GetAmmo(current_weapon) > 0 ? can_shoot = true : can_shoot = false;


	if (AttackState::IDLE == current_state && KEY_TAP(KEY::CTRL) && stiff_time <= 0.f && (can_shoot || current_weapon == WeaponState::KNIFE))
	{
		current_state = AttackState::FIRE;
		if(current_weapon != WeaponState::KNIFE)
			target->AddAmmo((AmmoType)current_weapon, -1);
		is_attack = true;
		BeginAttack();
	}

	if ((current_weapon == WeaponState::RIFFLE || current_weapon == WeaponState::MACHINEGUN))
	{
		if (AttackState::IDLE == current_state && KEY_PRESSED(KEY::CTRL) && stiff_time <= 0.f && can_shoot)
		{
			current_state = AttackState::FIRE;
			target->AddAmmo((AmmoType)current_weapon, -1);
			is_attack = true;
			BeginAttack();
		}
	}
	

}

void CPlayerHandScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerHandScript::SaveToFile(FILE* _File)
{
}

void CPlayerHandScript::LoadFromFile(FILE* _File)
{
}

void CPlayerHandScript::MiniMapCheck()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* TargetCam = pCurLevel->FindObjectByName(L"MainCamera");
	CGameObject* TargetRayCaster = pCurLevel->FindObjectByName(L"RayCaster2D");

	if (TargetCam && TargetRayCaster)
	{
		Vec3 CamPos = TargetCam->Transform()->GetRelativePos();
		Transform()->SetRelativePos(Vec3(CamPos.x, CamPos.y + 20.f, 100.f));
		Transform()->SetRelativeScale(Vec3(500.f, 550.f, 1.f));
	}
}

void CPlayerHandScript::StateCheck()
{
	if (!is_attack && stiff_time <= 0.f)
	{
		current_state = AttackState::IDLE;     
	}
	else
	{
		current_state = AttackState::FIRE;
	}
}


void CPlayerHandScript::SwitchWeapon()
{
	if (current_state == AttackState::IDLE)
	{
		if (KEY_TAP(KEY::_1))
			current_weapon = WeaponState::KNIFE;
		if (KEY_TAP(KEY::_2))
			current_weapon = WeaponState::PISTOL;
		if (KEY_TAP(KEY::_3))
			current_weapon = WeaponState::RIFFLE;
		if (KEY_TAP(KEY::_4))
			current_weapon = WeaponState::MACHINEGUN;
	}
}

void CPlayerHandScript::IdleAnim()
{
	if (current_state == AttackState::IDLE)
	{
		switch (current_weapon)
		{
		case WeaponState::KNIFE:
			FlipBookComponent()->Play(3, 1.f,false);
			break;
		case WeaponState::PISTOL:
			FlipBookComponent()->Play(4, 1.f, false);
			break;
		case WeaponState::RIFFLE:
			FlipBookComponent()->Play(5, 1.f, false);
			break;
		case WeaponState::MACHINEGUN:
			FlipBookComponent()->Play(6, 1.f, false);
			break;
		default:
			break;
		}
	}
}


void CPlayerHandScript::BeginAttack()
{
	switch (current_weapon)
	{
	case WeaponState::KNIFE:
		is_attack = false;
		stiff_time = 0.4f;
		FlipBookComponent()->Play(0, 10.f, false);
		knife_sound->Play(1, 1, true);
		break;
	case WeaponState::PISTOL:
		is_attack = false;
		stiff_time = 0.2f;
		MakeBullet();
		FlipBookComponent()->Play(1, 15.f, false);
		pistol_sound->Play(1, 1, true);
		break;
	case WeaponState::RIFFLE:
		is_attack = false;
		FlipBookComponent()->Play(2, 25.f, true);
		MakeBullet();
		stiff_time = 0.1f;
		riffle_sound->Play(1, 0.5f, true);
		break;
	case WeaponState::MACHINEGUN:
		is_attack = false;
		FlipBookComponent()->Play(7, 60.f, true);
		MakeBullet();
		MakeBullet();
		stiff_time = 0.05f;
		machinegun_sound->Play(1, 1, true);
		break;
	default:
		break;
	}
}

void CPlayerHandScript::MakeBullet()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* TargetRayCaster = pCurLevel->FindObjectByName(L"RayCaster2D");
	if (TargetRayCaster)
	{
		Vec3 TargetDir = TargetRayCaster->Transform()->GetWorldDir(DIR::UP);

		if (nullptr != bullet)
		{
			CGameObject* newBullet = bullet->Instantiate();
			newBullet->Transform()->SetRelativePos(TargetRayCaster->Transform()->GetRelativePos());
			CBulletScript* pScript = (CBulletScript*)newBullet->GetScripts()[0];
			pScript->SetDirection(Vec2(TargetDir.x, TargetDir.y));
			pScript->SetSpeed(2000.f);

			pCurLevel->AddObject((UINT)LAYER_NAME::PLAYER_BULLET, newBullet);
		}
	}
}


