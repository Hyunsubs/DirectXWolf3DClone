#include "pch.h"
#include "LayerUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

LayerUI::LayerUI()
{
	SetActive(false);
}

LayerUI::~LayerUI()
{
}


void LayerUI::Update()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	
	vector<wstring> LayerNames = {};
	vector<string>	LayerStrNames = {};
	char Buff[MAX_LAYER][256] = {};

	for (int i = 0; i < MAX_LAYER; i++)
	{
		LayerNames.push_back(pCurLevel->GetLayer(i)->GetName());
		LayerStrNames.push_back(string(LayerNames[i].begin(), LayerNames[i].end()));
		strcpy_s(Buff[i], LayerStrNames[i].c_str());
	}



	for (int i = 0; i < MAX_LAYER; i++)
	{
		string Label = "##LayerLabel";
		Label += (char)i;
		ImGui::Text("%d : ", i);
		ImGui::SameLine();
		ImGui::InputText(Label.c_str(), Buff[i], 256);
		LayerStrNames[i] = Buff[i];
	}

	for (int i = 0; i < MAX_LAYER; i++)
	{
		pCurLevel->GetLayer(i)->SetName(wstring(LayerStrNames[i].begin(), LayerStrNames[i].end()));
	}



}
