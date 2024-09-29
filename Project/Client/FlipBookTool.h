#pragma once
#include "EditorUI.h"


class FlipbookTool :
	public EditorUI
{
private:
	void LoadSpriteFile();
	void SaveFlipBook();
	void DisplayFlipBook();
	void ShowPreview();

	vector<Ptr<CSprite>> m_vecSprite;
	float preview_speed;
	float acc_time;
	bool is_play;
	int cur_frame;
	int max_frame;

public:
	virtual void Update() override;

public:
	FlipbookTool();
	~FlipbookTool();
};

