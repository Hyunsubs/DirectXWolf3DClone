#include "pch.h"
#include "CFlashFont.h"

#include <Engine/CFontMgr.h>

CFlashFont::CFlashFont()
	: CScript(SCRIPT_TYPE::FLASHFONT)
	, font_pos(Vec2())
	, font_size()
	, font_color()
	, flash_time()
	, on_off()
{
	AddScriptParam(SCRIPT_PARAM::VEC2, "font_pos", &font_pos);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "font_size", &font_size);
	AddScriptParam(SCRIPT_PARAM::VEC4, "font_color", &font_color);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "FlashTime", &flash_time);
}

CFlashFont::~CFlashFont()
{
}

void CFlashFont::Tick()
{
	acc_time += DT;

	if (flash_time <= acc_time)
	{
		acc_time = 0.f;
		on_off = !on_off;
	}
	
	if (on_off)
	{
		swprintf_s(sz_font, 50, GetOwner()->GetName().c_str());
		font = { sz_font, font_pos.x, font_pos.y, font_size, (UINT)FONT_RGBA(font_color.x, font_color.y, font_color.z, font_color.w) };
		CFontMgr::GetInst()->AddFont(&font);
	}
}

void CFlashFont::SaveToFile(FILE* _File)
{
	fwrite(&font_pos, sizeof(Vec2), 1, _File);
	fwrite(&font_size, sizeof(float), 1, _File);
	fwrite(&font_color, sizeof(Vec4), 1, _File);
	fwrite(&flash_time, sizeof(Vec4), 1, _File);
}

void CFlashFont::LoadFromFile(FILE* _File)
{
	fread(&font_pos, sizeof(Vec2), 1, _File);
	fread(&font_size, sizeof(float), 1, _File);
	fread(&font_color, sizeof(Vec4), 1, _File);
	fread(&flash_time, sizeof(Vec4), 1, _File);
}


