#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "ComponentUI.h"
#include "AssetUI.h"
#include "ScriptUI.h"


Inspector::Inspector()
	: m_TargetObject(nullptr)
	, m_arrComUI{}
	, m_arrAssetUI{}
{
	
}

Inspector::~Inspector()
{
}

void Inspector::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	// Object 가 보유하고 있는 컴포넌트에 대응하는 컴포넌트UI 가 활성화 됨
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;
			
		m_arrComUI[i]->SetTargetObject(_Object);
	}

	// Object 가 보유하고 있는 Script 마다 ScriptUI 가 배정됨
	if (nullptr == m_TargetObject)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetTargetScript(nullptr);
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_TargetObject->GetScripts();

		// 스크립트UI 개수가 부족하면 추가 생성
		if (m_vecScriptUI.size() < vecScripts.size())
		{
			CreateScriptUI((UINT)(vecScripts.size() - m_vecScriptUI.size()));
		}

		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if( i < vecScripts.size())
				m_vecScriptUI[i]->SetTargetScript(vecScripts[i]);
			else
				m_vecScriptUI[i]->SetTargetScript(nullptr);
		}		
	}

	// 에셋 UI 비활성화
	m_TargetAsset = nullptr;
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(nullptr);
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	if (nullptr == _Asset)
		return;

	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;
	
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(m_TargetAsset);
	}
}


void Inspector::Update()
{
	if (nullptr == m_TargetObject)
		return;

	SetTargetObject(m_TargetObject);

	// ===========
	// Object Name
	// ===========
	string strObjectName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	char NameBuff[256] = {};
	strcpy_s(NameBuff, strObjectName.c_str());
	ImGui::Text("Object Name");
	ImGui::SameLine(108);		
	ImGui::InputText("##ObjectName", NameBuff, 256);
	wstring wstrObjectName(NameBuff, &NameBuff[256]);
	m_TargetObject->SetName(wstrObjectName);
	
	// ======
	// Layer
	// ======
	int LayerIdx = m_TargetObject->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);	
	string LayerName = string(pLayer->GetName().begin(), pLayer->GetName().end());

	char buffer[50] = {};

	if(LayerName.empty())
		sprintf_s(buffer, 50, "%d : %s", LayerIdx, "None");
	else
		sprintf_s(buffer, 50, "%d : %s", LayerIdx, LayerName.c_str());

	ImGui::Text("Layer");
	ImGui::SameLine(108);
	ImGui::InputText("##LayerName", buffer, strlen(buffer), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::Button("+Layer"))
	{
		if (m_TargetObject->GetLayerIdx() >= MAX_LAYER - 1)
			return;
		else
			pCurLevel->AddObject(m_TargetObject->GetLayerIdx() + 1, m_TargetObject);
	}

	ImGui::SameLine();

	if (ImGui::Button("-Layer"))
	{
		if (m_TargetObject->GetLayerIdx() <= 0)
			return;
		else
			pCurLevel->AddObject(m_TargetObject->GetLayerIdx() - 1, m_TargetObject);
	}
}
