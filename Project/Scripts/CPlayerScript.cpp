#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CLayer.h>

#include "CPlayerStatusScript.h"

CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(400.f)
	, m_bFoundTileMap(false)
	, m_pTileMap(nullptr)
	, m_bInstantiated(false)
	, acc_time(0.f)
	, melt_start(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

CPlayerScript::CPlayerScript(CPlayerScript& _Origin)
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, acc_time(0.f)
	, melt_start(false)
{
	m_Speed = _Origin.m_Speed;
	m_bFoundTileMap = _Origin.m_bFoundTileMap;
	m_pTileMap = _Origin.m_pTileMap;
	m_bInstantiated = _Origin.m_bInstantiated;

	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
}

void CPlayerScript::Tick()
{
	extern int g_iTargetLevel;
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (nullptr != RayCaster2D() && nullptr != RayCaster2D()->GetTargetTileMap())
		m_pTileMap = RayCaster2D()->GetTargetTileMap();


	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vDir = Transform()->GetRelativeDir(DIR::UP);

	Vec3 vRot = Transform()->GetRelativeRotation();

	// 여기부터 수정
	if (KEY_TAP(KEY::SPACE))
	{
		Vec3 TargetPos = vPos + vDir * 100.f;
		CGameObject* pTargetObj = pLevel->FindObjectByName(L"TileMap");
		vector<tTileInfo> vecTile = m_pTileMap->TileMap()->GetTileInfo();
		tile_col = m_pTileMap->TileMap()->GetCol();
		tile_row = m_pTileMap->TileMap()->GetRow();
		tile_size = m_pTileMap->TileMap()->GetTileSize();
		int cur_col = TargetPos.x / tile_size.x;
		int cur_row = abs(TargetPos.y) / tile_size.y;

		if (vecTile[tile_col * cur_row + cur_col].ImgIdx == DOOR_TILE)
		{
			pTargetObj->TileMap()->ChangeTileInfo(cur_row, cur_col, 0);
			door_opening_sound->Play(1, 1, true);
		}

		if (vecTile[tile_col * cur_row + cur_col].ImgIdx == LEVER_TILE && g_iTargetLevel != 3)
		{
			melt_start = true;
		}

		if (melt_start)
		{
			Ptr<CPrefab> pref = FINDPREFAB(L"prefab\\MeltingScreen.pref");
			Instantiate(pref, 0, Vec3(), L"MeltingScreen");
			melt_start = false;
			screen_instantiate = true;
		}
	}

	if (screen_instantiate)
	{
		acc_time += DT;
	}

	if (acc_time >= 1.f)
	{
		Ptr<CPrefab> pref = FINDPREFAB(L"prefab\\InterMission.pref");
		Vec3 targetPos = Transform()->GetWorldPos();
		Instantiate(pref, (UINT)LAYER_NAME::UI, Vec3(targetPos.x, targetPos.y + 400.f, targetPos.z), L"InterMission");
		clear_sound->Play(1, 1, true);
		vector<CGameObject*> monsters = pLevel->GetLayer((UINT)LAYER_NAME::MONSTER)->GetObjects();
		for (int i = 0; i < monsters.size(); i++)
		{
			DeleteObject(monsters[i]);
		}
		acc_time = 0.f;
		DeleteObject(GetOwner());
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vRot.z += DT * XM_PI * 0.5f;
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		vRot.z -= DT * XM_PI * 0.5f;
	}

	if (KEY_PRESSED(KEY::UP))
	{
		vPos += DT * vDir * m_Speed;
	}
	
	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos -= DT * vDir * m_Speed;

	}


	if (m_pTileMap)
	{
		tile_col = m_pTileMap->TileMap()->GetCol();
		tile_row = m_pTileMap->TileMap()->GetRow();
		tile_size = m_pTileMap->TileMap()->GetTileSize();

		int cur_col = vPos.x / tile_size.x;
		int cur_row = abs(vPos.y) / tile_size.y;

		vector<tTileInfo> vecTile = m_pTileMap->TileMap()->GetTileInfo();
		vecTile[tile_col * cur_row + cur_col].ImgIdx == 0 ? m_bCanMove = true : m_bCanMove = false;
	}


	if (m_bCanMove)
	{
		Transform()->SetRelativePos(vPos);
		Transform()->SetRelativeRotation(vRot);
	}

	CGameObject* pMainCam = pLevel->FindObjectByName(L"MainCamera");
	if (nullptr != pMainCam)
	{
		Vec3 CurrentPos = Transform()->GetWorldPos();
		pMainCam->Transform()->SetRelativePos(CurrentPos);
	}

	CGameObject* pRenderer = pLevel->FindObjectByName(L"RayCasterRenderer");
	if (nullptr != pRenderer)
	{
		Vec3 CurrentPos = Transform()->GetWorldPos();
		pRenderer->Transform()->SetRelativePos(Vec3(CurrentPos.x, CurrentPos.y, 200.f));
	}

}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if ((UINT)LAYER_NAME::MONSTER_BULLET == _OtherObject->GetLayerIdx())
	{
		DeleteObject(_OtherObject);
		CGameObject* status = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"ScoreBoard");
		if (status)
		{
			vector<CScript*> scripts = status->GetScripts();
			for (int i = 0; i < scripts.size(); i++)
			{
				if (SCRIPT_TYPE::PLAYERSTATUSSCRIPT == scripts[i]->GetScriptType())
				{
					CPlayerStatusScript* targetStatus = (CPlayerStatusScript*)scripts[i];
					targetStatus->AddHp(-1);
				}
			}
		}

		Ptr<CPrefab> redScreen = FINDPREFAB(L"prefab\\RedScreen.pref");
		Instantiate(redScreen, 0, Vec3(), L"YellowScreen");
		attacked_sound->Play(1, 1, false);
	}

}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
