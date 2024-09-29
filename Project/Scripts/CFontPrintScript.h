#pragma once
#include <Engine/CScript.h>

class CFontPrintScript
	: public CScript
{
private:
	Vec2 font_pos;
	Vec4 font_color;
	float font_size;
	wchar_t sz_font[50];

	FontData font;

	virtual void Tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE(CFontPrintScript)
	CFontPrintScript();
	~CFontPrintScript();

};

