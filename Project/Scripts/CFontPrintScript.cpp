#include "pch.h"
#include "CFontPrintScript.h"
#include <Engine/CFontMgr.h>


CFontPrintScript::CFontPrintScript()
	: CScript(SCRIPT_TYPE::FONTPRINTSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::VEC2, "font_pos", &font_pos);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "font_size", &font_size);
	AddScriptParam(SCRIPT_PARAM::VEC4, "font_color", &font_color);
}

CFontPrintScript::~CFontPrintScript()
{
}


void CFontPrintScript::Tick()
{
	swprintf_s(sz_font, 50, GetOwner()->GetName().c_str());
	font = { sz_font, font_pos.x, font_pos.y, font_size, (UINT)FONT_RGBA(font_color.x, font_color.y, font_color.z, font_color.w) };
	CFontMgr::GetInst()->AddFont(&font);
}

void CFontPrintScript::SaveToFile(FILE* _File)
{
	fwrite(&font_pos, sizeof(Vec2), 1, _File);
	fwrite(&font_size, sizeof(float), 1, _File);
	fwrite(&font_color, sizeof(Vec4), 1, _File);
}

void CFontPrintScript::LoadFromFile(FILE* _File)
{
	fread(&font_pos, sizeof(Vec2), 1, _File);
	fread(&font_size, sizeof(float), 1, _File);
	fread(&font_color, sizeof(Vec4), 1, _File);
}


