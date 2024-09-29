#include "pch.h"
#include "RayCaster2DUI.h"

#include <Engine/CRayCaster2D.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CAssetMgr.h>

#include "ListUI.h"
#include "CEditorMgr.h"


RayCaster2DUI::RayCaster2DUI()
	: ComponentUI(COMPONENT_TYPE::RAYCASTER2D)
{


}

RayCaster2DUI::~RayCaster2DUI()
{


}


void RayCaster2DUI::Update()
{
	Title();
	
	CRayCaster2D* pRayCaster = GetTargetObject()->RayCaster2D();

	Vec2 intersection_point = pRayCaster->GetCurrentIntersection();

	ImGui::Text("Current Intersect : ");
	ImGui::InputFloat2("##CurIntersect", intersection_point, "%.3f", ImGuiInputTextFlags_ReadOnly);

	string CurrentTileMap = "None";
	if (nullptr != pRayCaster->GetTargetTileMap())
	{
		CurrentTileMap = string(pRayCaster->GetTargetTileMap()->GetName().begin(), pRayCaster->GetTargetTileMap()->GetName().end());
	}
	
	char buff[256] = {};
	strcpy_s(buff, CurrentTileMap.c_str());
	ImGui::Text("Current Target TileMap : ");
	ImGui::InputText("##CurrentTileMap", buff, ImGuiInputTextFlags_ReadOnly);

	
	ImGui::SameLine();
	if (ImGui::Button("##TileMapBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Objects");
		
		vector<wstring> vecObjNames = CLevelMgr::GetInst()->GetCurrentLevel()->GetAllObjectNames();
		vector<string> vecStringNames = {};
		for (int i = 0; i < vecObjNames.size(); i++)
		{
			vecStringNames.push_back(string(vecObjNames[i].begin(), vecObjNames[i].end()));
		}
		pListUI->AddList(vecStringNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&RayCaster2DUI::SetTargetObj);
		pListUI->SetActive(true);
	}

	int rayAmount = pRayCaster->GetRayAmount();
	ImGui::InputInt("##RayAmount", &rayAmount);
	pRayCaster->SetRayAmount(rayAmount);

	float m_Fov = pRayCaster->GetFOV();
	ImGui::InputFloat("##FovSetting", &m_Fov);
	pRayCaster->SetFOV(m_Fov);

}

void RayCaster2DUI::SetTargetObj(DWORD_PTR _ListUI)
{
	CRayCaster2D* pRayCaster = GetTargetObject()->RayCaster2D();

	ListUI* pListUI = (ListUI*)_ListUI;

	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		pRayCaster->SetTargetTileMap(nullptr);
		return;
	}

	wstring strObjName = wstring(strName.begin(), strName.end());


	pRayCaster->SetTargetTileMap(CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(strObjName));
}




