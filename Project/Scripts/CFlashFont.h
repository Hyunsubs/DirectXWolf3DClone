#pragma once
#include <Engine/CScript.h>
// 일정 시간마다 반짝이게 하기

class CFlashFont
	: public CScript
{
private:
	Vec2 font_pos;
	Vec4 font_color;
	float font_size;
	wchar_t sz_font[50];

	FontData font;
	
	float flash_time;
	float acc_time;
	bool on_off;

public:
	virtual void Tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE(CFlashFont)
	CFlashFont();
	~CFlashFont();

};

