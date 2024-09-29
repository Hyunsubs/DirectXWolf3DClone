#include "pch.h"
#include "CSpriteScript.h"

CSpriteScript::CSpriteScript()
	: CScript(UINT(SCRIPT_TYPE::SPRITESCRIPT))
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Texture", &m_TargetTexture);
	AddScriptParam(SCRIPT_PARAM::INT, "IsFlipBook", &m_IsFlipBook);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "offsetY", &m_offsetY);
	AddScriptParam(SCRIPT_PARAM::INT, "IsCollision", &m_IsCollision);
}

CSpriteScript::CSpriteScript(CSpriteScript& _Origin)
	: CScript(UINT(SCRIPT_TYPE::SPRITESCRIPT))
{
	m_TargetTexture = _Origin.m_TargetTexture;

	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Texture", &m_TargetTexture);
	AddScriptParam(SCRIPT_PARAM::INT, "IsFlipBook", &m_IsFlipBook);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "offsetY", &m_offsetY);
	AddScriptParam(SCRIPT_PARAM::INT, "IsCollision", &m_IsCollision);
}

CSpriteScript::~CSpriteScript()
{
}

void CSpriteScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
}

void CSpriteScript::Tick()
{
	if (nullptr != FlipBookComponent())
		FlipBookComponent()->Play(0, 1.f, true);
}

void CSpriteScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSpriteScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_TargetTexture, _File);
	fwrite(&m_IsFlipBook, sizeof(int), 1, _File);
	fwrite(&m_offsetY, sizeof(float), 1, _File);
	fwrite(&m_IsCollision, sizeof(int), 1, _File);
}

void CSpriteScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_TargetTexture, _File);
	fread(&m_IsFlipBook, sizeof(int), 1, _File);
	fread(&m_offsetY, sizeof(float), 1, _File);
	fread(&m_IsCollision, sizeof(int), 1, _File);
}




