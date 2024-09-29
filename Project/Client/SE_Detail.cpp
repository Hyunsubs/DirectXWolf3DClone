#include "pch.h"
#include "SE_Detail.h"

#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

#include "SE_AtlasView.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CSprite.h>

#include <Engine/CPathMgr.h>

SE_Detail::SE_Detail()
{
}

SE_Detail::~SE_Detail()
{
}


void SE_Detail::Init()
{
	
}

void SE_Detail::Update()
{
	Atlas();

	AtlasInfo();

	ImGui::BeginDisabled(nullptr == m_AtlasTex || m_BackgroundSize.x <= 0.f || m_BackgroundSize.y <= 0.f);
	if (ImGui::Button("Save Sprite"))
	{
		wstring contentpath = CPathMgr::GetInst()->GetContentPath() + L"Sprites\\";
		wstring TargetPath = FileSaveSystem(L"sprite", contentpath);

		if (L"" != TargetPath)
		{
			Ptr<CSprite> pSprite = new CSprite;
			pSprite.Get()->SetAtlasTex(m_AtlasTex);
			pSprite.Get()->SetLeftTop(Vec2(GetAtlasView()->GetPixelLT().x, GetAtlasView()->GetPixelLT().y));
			pSprite.Get()->SetSlice(Vec2(GetAtlasView()->GetPixelRB().x - GetAtlasView()->GetPixelLT().x, GetAtlasView()->GetPixelRB().y - GetAtlasView()->GetPixelLT().y));
			pSprite.Get()->SetBackground(m_BackgroundSize);
			pSprite.Get()->SetOffset(m_Offset);
			pSprite->Save(TargetPath);
		}
	}

	ImGui::EndDisabled();
}

void SE_Detail::DrawCanvas()
{
	if (m_BackgroundSize.x <= 0.f || m_BackgroundSize.y <= 0.f)
		return;
	static ImVector<ImVec2> points;
	static ImVec2 scrolling(0.0f, 0.0f);
	static bool adding_line = false;

	// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImVec2(m_BackgroundSize.x, m_BackgroundSize.y);   // Resize canvas to what's available
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	// Add first and second point
	if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		points.push_back(mouse_pos_in_canvas);
		points.push_back(mouse_pos_in_canvas);
		adding_line = true;
	}
	if (adding_line)
	{
		points.back() = mouse_pos_in_canvas;
		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
			adding_line = false;
	}


	if (nullptr != m_AtlasTex)
	{
		// Draw grid + all lines in the canvas
		draw_list->PushClipRect(canvas_p0, canvas_p1, true);

		// 캔버스에 이미지 추가
		float width = GetAtlasView()->GetPixelRB().x - GetAtlasView()->GetPixelLT().x;
		float height = GetAtlasView()->GetPixelRB().y - GetAtlasView()->GetPixelLT().y;

		ImVec2 uv_min = ImVec2(GetAtlasView()->GetPixelLT().x / m_AtlasTex->Width(), GetAtlasView()->GetPixelLT().y / m_AtlasTex->Height());
		ImVec2 uv_max = ImVec2(GetAtlasView()->GetPixelRB().x / m_AtlasTex->Width(), GetAtlasView()->GetPixelRB().y / m_AtlasTex->Height());

		ImVec2 image_start = ImVec2(origin.x + (canvas_sz.x / 2) - (width/2) + m_Offset.x, origin.y + (canvas_sz.y / 2) - (height / 2) + m_Offset.y);
		ImVec2 image_end = ImVec2(image_start.x + width, image_start.y + height);

		draw_list->AddImage(m_AtlasTex->GetSRV().Get(), 
							image_start,
							image_end, uv_min, uv_max);

		draw_list->PopClipRect();
	}
}


void SE_Detail::Atlas()
{	
	string TexName;		

	if (nullptr != m_AtlasTex)
		TexName = string(m_AtlasTex->GetKey().begin(), m_AtlasTex->GetKey().end());

	ImGui::Text("Atlas Texture");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AtlasTex", (char*)TexName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				SetAtlasTex((CTexture*)pAsset.Get());				
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##AtlasTexBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Texture");
		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&SE_Detail::SelectTexture);
		pListUI->SetActive(true);
	}

	ImGui::Text("BackGroundSize x,y : ");
	ImGui::SameLine();
	ImGui::InputFloat2("##BackGroundSize", m_BackgroundSize);

	ImGui::Text("Offset x,y : ");
	ImGui::SameLine();
	ImGui::InputFloat2("##OffsetSize", m_Offset);

	ImGui::Text("Preview Image");

	DrawCanvas();
	

}

void SE_Detail::AtlasInfo()
{
	// 해상도
	UINT width = 0;
	UINT height = 0;

	if (nullptr != m_AtlasTex)
	{
		width = m_AtlasTex->Width();
		height = m_AtlasTex->Height();
	}

	char buff[50] = {};
	sprintf_s(buff, "%d", width);

	ImGui::Text("Atlas Width");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureWidth", buff, 50, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(buff, "%d", height);
	ImGui::Text("Atlas Height");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureHeight", buff, 50, ImGuiInputTextFlags_ReadOnly);
}

void SE_Detail::SetAtlasTex(Ptr<CTexture> _Tex)
{
	m_AtlasTex = _Tex;
	GetAtlasView()->SetAtlasTex(m_AtlasTex);
}



void SE_Detail::SelectTexture(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_AtlasTex = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	SetAtlasTex(pTex);	
}