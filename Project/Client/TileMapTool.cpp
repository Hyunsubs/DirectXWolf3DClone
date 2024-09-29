#include "pch.h"
#include "TileMapTool.h"
#include <Engine/CAssetMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CPrefab.h>
#include <Engine/CTileMap.h>
#include <Engine/CPathMgr.h>
#include <Engine/CTransform.h>

TileMapTool::TileMapTool()
{
    SetActive(false);
}

TileMapTool::~TileMapTool()
{
}

int TileMapTool::image_idx_x = 0;
int TileMapTool::image_idx_y = 0;
int TileMapTool::column = 0;
int TileMapTool::row = 0;
int TileMapTool::TileX = 0;
int TileMapTool::TileY = 0;


void TileMapTool::Update()
{
    ShowCurTexture();
    DisplayCurrentTile();
    RenderCanvas();
    ClearTile();
    ImGui::SameLine();
    SaveTileMapPref();


}

void TileMapTool::DisplayCurrentTile()
{
    ImGui::Text("Current Tile Image Col Row : ");
    ImGui::SameLine();
    ImGui::InputInt("##CurImageCol", &image_idx_x);
    ImGui::Text("\t\t\t\t\t\t\t ");
    ImGui::SameLine();
    ImGui::InputInt("##CurImageRow", &image_idx_y);

    if (image_idx_x > image_slice_column)
        image_idx_x = image_slice_column;
    if (image_idx_y > image_slice_row)
        image_idx_y = image_slice_row;


    ImVec2 img_min_uv = ImVec2((float)(TileX * image_idx_x) / m_CurTexture->Width(), (float)(TileY * image_idx_y) / m_CurTexture->Height());
    ImVec2 img_max_uv = ImVec2((float)(TileX * image_idx_x + TileX) / m_CurTexture->Width(), (float)(TileY * image_idx_y + TileY) / m_CurTexture->Height());

    ImGui::Image(m_CurTexture->GetSRV().Get(), ImVec2(TileX, TileY), img_min_uv, img_max_uv);

}

void TileMapTool::ClearTile()
{
    if (ImGui::Button("ClearTile"))
    {
        m_TileMap.clear();
    }
}

void TileMapTool::SaveTileMapPref()
{
    if (ImGui::Button("SaveTileMapPref"))
    {
        CGameObject* pTileMapObj = new CGameObject;
        pTileMapObj->AddComponent(new CTransform);
        pTileMapObj->AddComponent(new CTileMap);
        pTileMapObj->TileMap()->SetAtlasTexture(m_CurTexture);
        pTileMapObj->TileMap()->SetTileSize(Vec2(TileX, TileY));
        pTileMapObj->TileMap()->SetAtlasTileSize(Vec2(TileX, TileY));
        pTileMapObj->TileMap()->SetRowCol(row, column);

        for (int i = 0; i < m_TileMap.size(); i++)
        {
            if (m_TileMap[i].x >= column || m_TileMap[i].x < 0 || m_TileMap[i].y >= row || m_TileMap[i].y < 0)
                continue;
            pTileMapObj->TileMap()->ChangeTileInfo(m_TileMap[i].y, m_TileMap[i].x, m_TileMap[i].w * image_slice_column + m_TileMap[i].z + 1);
        }

        Ptr<CPrefab> pPref = new CPrefab;
        pPref->SetProtoObject(pTileMapObj);

        // common dialog box structure, setting all fields to 0 is important
        OPENFILENAME ofn = { 0 };
        TCHAR szFile[260] = { 0 };
        // Initialize remaining fields of OPENFILENAME structure
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = CEngine::GetInst()->GetMainWnd();
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = NULL;
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetSaveFileName(&ofn) == TRUE)
        {
            pPref->Save(szFile);
        }
    }
}


void TileMapTool::ShowCurTexture()
{
    static int item_current_idx = 0;
    vector<string> vec_TexturesNames = {};
    CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vec_TexturesNames);

    ImGui::Text("TileTexture : ");
    ImGui::SameLine(120);
    if (ImGui::BeginCombo("##ImageSelect", vec_TexturesNames[item_current_idx].c_str()))
    {
        for (int n = 0; n < vec_TexturesNames.size(); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(vec_TexturesNames[n].c_str(), is_selected))
                item_current_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::Separator();

    m_CurTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(vec_TexturesNames[item_current_idx].begin(), vec_TexturesNames[item_current_idx].end()));
    ImGui::Image(m_CurTexture->GetSRV().Get(), ImVec2(300.f, 300.f));
}

void TileMapTool::RenderCanvas()
{
    image_width = m_CurTexture->Width();
    image_height = m_CurTexture->Height();

    if (image_width == 0 || image_height == 0 || TileX == 0 || TileY == 0)
    {
        image_slice_column = 0;
        image_slice_row = 0;
    }
    else
    {
        image_slice_column = image_width / TileX;
        image_slice_row = image_height / TileY;
    }

    ImGui::Text("Tile Column : ");
    ImGui::SameLine();
    ImGui::InputInt("##inputColumn", &column);


    ImGui::Text("Tile Row : ");
    ImGui::SameLine();
    ImGui::InputInt("##inputRow", &row);


    ImGui::Text("TileX : ");
    ImGui::SameLine();
    ImGui::InputInt("##TileXInput", &TileX);

    ImGui::Text("TileY : ");
    ImGui::SameLine();
    ImGui::InputInt("##TileYInput", &TileY);

    static ImVector<ImVec2> points;
    static ImVec2 scrolling(0.0f, 0.0f);
    static bool adding_line = false;

    // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImVec2(1280.f, 720.f);   // Resize canvas to what's available
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

    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, true))
    {
        scrolling.x += io.MouseDelta.x;
        scrolling.y += io.MouseDelta.y;
    }

    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            draw_list->AddRect(ImVec2(origin.x + (j * TileX), origin.y + (i * TileY)), ImVec2(origin.x + (j * TileX) + TileX, origin.y + (i * TileY) + TileY), IM_COL32(0, 0, 255, 255), 1.0f);
        }
    }
    draw_list->PopClipRect();

    // 마우스 왼쪽 클릭 체크
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && is_hovered)
    {
        if (TileX == 0 || TileY == 0)
            return;

        int click_col = (int)(mouse_pos_in_canvas.x) / TileX;
        int click_row = (int)(mouse_pos_in_canvas.y) / TileY;
        int max_x = column * TileX;
        int max_y = row * TileY;

        if (mouse_pos_in_canvas.x > max_x || mouse_pos_in_canvas.y > max_y || mouse_pos_in_canvas.x < 0.f || mouse_pos_in_canvas.y < 0.f)
            return;
        Vec4 targetColRow = Vec4(click_col, click_row, image_idx_x, image_idx_y);
        
        m_TileMap.push_back(targetColRow);

    }

    for (int i = 0; i < m_TileMap.size(); i++)
    {
        int click_col = m_TileMap[i].x;
        int click_row = m_TileMap[i].y;
        ImVec2 target_pos = ImVec2(origin.x + click_col * TileX, origin.y + click_row * TileY);
        ImVec2 img_min_uv = ImVec2((float)(TileX * m_TileMap[i].z) / m_CurTexture->Width(), (float)(TileY * m_TileMap[i].w) / m_CurTexture->Height());
        ImVec2 img_max_uv = ImVec2((float)(TileX * m_TileMap[i].z + TileX) / m_CurTexture->Width(), (float)(TileY * m_TileMap[i].w + TileY) / m_CurTexture->Height());
        if (click_col >= column || click_row >= row)
        {
            m_TileMap.erase(m_TileMap.begin() + i);
        }

        
        if (click_col < column && click_row < row)
        {
            draw_list->PushClipRect(canvas_p0, canvas_p1, true);
            draw_list->AddImage(m_CurTexture->GetSRV().Get(),
                target_pos,
                ImVec2(target_pos.x + TileX, target_pos.y + TileY),
                img_min_uv, img_max_uv);
            draw_list->PopClipRect();
        }
    }


}



