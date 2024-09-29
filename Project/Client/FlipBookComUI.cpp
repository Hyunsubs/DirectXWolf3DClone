#include "pch.h"
#include "FlipBookComUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CFlipBookComponent.h>
#include <Engine/CAssetMgr.h>
#include "ListUI.h"
#include "CEditorMgr.h"

FlipBookComUI::FlipBookComUI()
	: ComponentUI(COMPONENT_TYPE::FLIPBOOKCOMPONENT)
{
	
}

FlipBookComUI::~FlipBookComUI()
{
}


void FlipBookComUI::Update()
{
	Title();

	CFlipBookComponent* pFlip = GetTargetObject()->FlipBookComponent();
	vector<Ptr<CFlipBook>> m_vecFlipBook = pFlip->GetAllFlipBook();

	for (int i = 0; i < m_vecFlipBook.size(); i++)
	{
		Ptr<CFlipBook> curFlip = m_vecFlipBook[i];
		string FlipBookName = string(curFlip->GetKey().begin(), curFlip->GetKey().end());
		ImGui::Text("FlipBook %d :", i);
		ImGui::SameLine();
		ImGui::InputText("##FlipbookName", FlipBookName.data(), ImGuiInputTextFlags_ReadOnly);
	}

	if (ImGui::Button("AddFlipBook"))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("FlipBook");
		vector<string> vecFlipBookNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::FLIPBOOK, vecFlipBookNames);
		pListUI->AddList(vecFlipBookNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&FlipBookComUI::SelectFlip);
		pListUI->SetActive(true);
	}


}

void FlipBookComUI::SelectFlip(DWORD_PTR _ListUI)
{
	CFlipBookComponent* pFlip = GetTargetObject()->FlipBookComponent();
	vector<Ptr<CFlipBook>> m_vecFlipBook = pFlip->GetAllFlipBook();

	ListUI* pListUI = (ListUI*)_ListUI;
	string FlipName = pListUI->GetSelectName();

	if (FlipName == "None")
	{
		return;
	}

	wstring strAssetName = wstring(FlipName.begin(), FlipName.end());
	Ptr<CFlipBook> TargetFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(strAssetName);

	assert(TargetFlip.Get());

	pFlip->AddFlipBook(m_vecFlipBook.size(), TargetFlip);
}



