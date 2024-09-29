#include "pch.h"
#include "TileMapUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>

TileMapUI::TileMapUI()
	: ComponentUI(COMPONENT_TYPE::TILEMAP)
{
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::Update()
{
	Title();
}