#include "pch.h"
#include "CBulletScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CBulletScript::CBulletScript()
	: CScript(SCRIPT_TYPE::BULLETSCRIPT)
	, Speed(0.f)
	, Direction(Vec2())
	, life(5.f)
{
}

CBulletScript::CBulletScript(CBulletScript& _Origin)
	: CScript(SCRIPT_TYPE::BULLETSCRIPT)
	, Speed(0.f)
	, Direction(Vec2())
	, life(5.f)
{
}

CBulletScript::~CBulletScript()
{
}


void CBulletScript::Begin()
{
}

void CBulletScript::Tick()
{
	life -= DT;

	Vec3 CurPos = Transform()->GetRelativePos();
	WallCheck(CurPos);
	CurPos.x += Direction.x * DT * Speed;
	CurPos.y += Direction.y * DT * Speed;
	Transform()->SetRelativePos(Vec3(CurPos.x, CurPos.y, 400.f));

	if (life <= 0.f && !GetOwner()->IsDead())
	{
		DeleteObject(GetOwner());
	}
}

void CBulletScript::WallCheck(Vec3 _Pos)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* TileMapInfo = pCurLevel->FindObjectByName(L"TileMap");

	int row = TileMapInfo->TileMap()->GetRow();
	int col = TileMapInfo->TileMap()->GetCol();

	Vec2 tile_size = TileMapInfo->TileMap()->GetTileSize();
	int cur_row = abs(_Pos.y) / tile_size.y;
	int cur_col = _Pos.x / tile_size.x;

	vector<tTileInfo> tileInfo = TileMapInfo->TileMap()->GetTileInfo();

	if (cur_row < row && cur_col < col)
	{
		if (tileInfo[col * cur_row + cur_col].ImgIdx != 0)
			DeleteObject(GetOwner());
	}

}

void CBulletScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CBulletScript::SaveToFile(FILE* _File)
{
}
void CBulletScript::LoadFromFile(FILE* _File)
{
}

