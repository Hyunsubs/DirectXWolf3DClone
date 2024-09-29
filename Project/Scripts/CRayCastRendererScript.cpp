#include "pch.h"
#include "CRayCastRendererScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CTileMap.h>
#include <Engine/CRayCaster2D.h>
#include <Engine/CStructuredBuffer.h>


CRayCastRendererScript::CRayCastRendererScript()
	: CScript(SCRIPT_TYPE::RAYCASTRENDERERSCRIPT)
{
}


CRayCastRendererScript::~CRayCastRendererScript()
{
}

void CRayCastRendererScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
}

void CRayCastRendererScript::Tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* pRayCaster = pCurLevel->FindObjectByName(L"RayCaster2D");
	if (pRayCaster && pRayCaster->RayCaster2D())
	{
		Transform()->SetRelativePos(pRayCaster->Transform()->GetWorldPos().x, pRayCaster->Transform()->GetWorldPos().y, 600.f);
		CStructuredBuffer* RayBuffer = pRayCaster->RayCaster2D()->GetRayBuffer();
		RayBuffer->Binding(18);
	}

	CGameObject* pTileMap = pCurLevel->FindObjectByName(L"TileMap");
	if (pTileMap && pTileMap->TileMap())
	{
		CStructuredBuffer* TileBuffer = pTileMap->TileMap()->GetTileBuffer();
		GetRenderComponent()->GetMaterial()->SetTexParam(TEX_0, pTileMap->TileMap()->GetAtlasTexture());
		GetRenderComponent()->GetMaterial()->SetScalarParam(INT_0, pTileMap->TileMap()->GetAtlasMaxRow());
		GetRenderComponent()->GetMaterial()->SetScalarParam(INT_1, pTileMap->TileMap()->GetAtlasMaxCol());
		GetRenderComponent()->GetMaterial()->SetScalarParam(VEC2_0, pTileMap->TileMap()->GetTileSliceUV());
		GetRenderComponent()->GetMaterial()->SetScalarParam(VEC2_1, Vec2(pTileMap->TileMap()->GetRow(), pTileMap->TileMap()->GetCol()));
		TileBuffer->Binding(19);
	}


}

void CRayCastRendererScript::SaveToFile(FILE* _File)
{
}

void CRayCastRendererScript::LoadFromFile(FILE* _File)
{
}



