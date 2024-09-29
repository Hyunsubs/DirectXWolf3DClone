#include "pch.h"
#include "CLevelStarter.h"

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CCollisionMgr.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CSetColorCS.h>
#include <Engine/CStructuredBuffer.h>


#include "CLevelSaveLoad.h" 

void CLevelStarter::CreateLevel()
{
	// Level »ý¼º
	wstring contentPath = CPathMgr::GetInst()->GetContentPath();
	contentPath += L"level\\StartCredit.lv";
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(contentPath);
	
#ifdef _DEBUG
	ChangeLevel(pLevel, LEVEL_STATE::STOP);
#else
	ChangeLevel(pLevel, LEVEL_STATE::PLAY);
#endif

}

