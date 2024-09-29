#pragma once
#include "EditorUI.h"
#include <Engine\CTexture.h>

class TileMapTool :
	public EditorUI
{
private:
	Ptr<CTexture> m_CurTexture;
	vector<Vec4> m_TileMap;
	Vec2 m_ClickedPos;
	wstring TileName;

	static int image_idx_x;
	static int image_idx_y;
	static int column;
	static int row;

	static int TileX;
	static int TileY;

	int image_width;
	int image_height;

	int image_slice_column = 0;
	int image_slice_row = 0;

	void ShowCurTexture();
	void RenderCanvas();
	void DisplayCurrentTile();
	void ClearTile();
	void SaveTileMapPref();

public:
	virtual void Update() override;



public:
	TileMapTool();
	~TileMapTool();
};

