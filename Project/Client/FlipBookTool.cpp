#include "pch.h"
#include "FlipbookTool.h"
#include <Engine/CAssetMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CEngine.h>
#include <Engine/CTimeMgr.h>

FlipbookTool::FlipbookTool()
{
    SetActive(false);
}

FlipbookTool::~FlipbookTool()
{
}


void FlipbookTool::LoadSpriteFile()
{
    if (ImGui::Button("Load Sprite"))
    {
        wstring contentPath = CPathMgr::GetInst()->GetContentPath();
        contentPath += L"Sprites\\";
        wstring targetPath = L"";
        targetPath = FileLoadSystem(L"sprite", contentPath);
        if (L"" != targetPath)
        {
            targetPath.erase(targetPath.begin(), targetPath.begin() + CPathMgr::GetInst()->GetContentPath().size());
            m_vecSprite.push_back(FINDSPRITE(targetPath));
        }
    }
}

void FlipbookTool::SaveFlipBook()
{
    if (ImGui::Button("Save FlipBook"))
    {
        wstring contentPath = CPathMgr::GetInst()->GetContentPath();
        contentPath += L"Animation\\";
        wstring targetPath = L"";
        targetPath = FileSaveSystem(L"flip", contentPath);
        if (L"" != targetPath)
        {
            Ptr<CFlipBook> pFlipBook = new CFlipBook;
            for (int i = 0; i < m_vecSprite.size(); i++)
            {
                pFlipBook->AddSprite(FINDSPRITE(m_vecSprite[i]->GetKey()));
            }

            pFlipBook->Save(targetPath);
        }
    }
}

void FlipbookTool::DisplayFlipBook()
{
    if (!m_vecSprite.empty())
    {
        for (int i = 0; i < m_vecSprite.size(); i++)
        {
            Ptr<CTexture> pTex = m_vecSprite[i].Get()->GetAtlasTexture();
            ImVec2 uv_min = ImVec2(m_vecSprite[i].Get()->GetLeftTopUV().x, m_vecSprite[i].Get()->GetLeftTopUV().y);
            ImVec2 uv_max = ImVec2(m_vecSprite[i].Get()->GetLeftTopUV().x + m_vecSprite[i].Get()->GetSliceUV().x, m_vecSprite[i].Get()->GetLeftTopUV().y + m_vecSprite[i].Get()->GetSliceUV().y);
            ImVec2 image_size = ImVec2((uv_max.x - uv_min.x) * pTex->Width(), (uv_max.y - uv_min.y) * pTex->Height());
            ImVec2 background_size = ImVec2(m_vecSprite[i].Get()->GetBackgroundUV().x * pTex->Width(), m_vecSprite[i].Get()->GetBackgroundUV().y * pTex->Height());
            ImGui::Dummy(ImVec2(0.f, background_size.y - image_size.y));
            ImGui::SameLine();
            ImGui::Image(pTex->GetSRV().Get(), image_size, uv_min, uv_max);
            ImGui::SameLine();
        }
    }
}

void FlipbookTool::ShowPreview()
{
    ImGui::Text("Preview Speed : ");
    ImGui::InputFloat("##PreviewSpeed", &preview_speed);

    if (ImGui::Button("Play"))
    {
        is_play = true;
        acc_time = 0.f;
    }

    ImGui::SameLine();

    if (ImGui::Button("Stop"))
    {
        is_play = false;
        acc_time = 0.f;
    }

    if (is_play && preview_speed > 0.f && !m_vecSprite.empty())
    {
        acc_time += CTimeMgr::GetInst()->GetEngineDeltaTime();
        Ptr<CSprite> cur_sprite = m_vecSprite[cur_frame];
        Ptr<CTexture> cur_sprite_tex = cur_sprite->GetAtlasTexture();
        ImVec2 LeftTopSize = ImVec2(cur_sprite->GetLeftTopUV().x * cur_sprite_tex->Width(), cur_sprite->GetLeftTopUV().y * cur_sprite_tex->Height());
        ImVec2 SliceSize = ImVec2(cur_sprite->GetSliceUV().x * cur_sprite_tex->Width(), cur_sprite->GetSliceUV().y * cur_sprite_tex->Height());
        ImVec2 image_size = ImVec2(cur_sprite->GetBackgroundUV().x * cur_sprite_tex->Width(), cur_sprite->GetBackgroundUV().y * cur_sprite_tex->Height());
        ImVec2 uv_min = ImVec2(cur_sprite.Get()->GetLeftTopUV().x, cur_sprite.Get()->GetLeftTopUV().y);
        ImVec2 uv_max = ImVec2(cur_sprite.Get()->GetLeftTopUV().x + cur_sprite.Get()->GetSliceUV().x, cur_sprite.Get()->GetLeftTopUV().y + cur_sprite.Get()->GetSliceUV().y);
        ImGui::Image(cur_sprite_tex->GetSRV().Get(), image_size, uv_min, uv_max);
    }

    if (acc_time > preview_speed)
    {
        acc_time = 0.f;
        cur_frame++;
    }

    if (cur_frame >= m_vecSprite.size())
    {
        cur_frame = 0;
    }

}

void FlipbookTool::Update()
{
    LoadSpriteFile();
    DisplayFlipBook();
    ImGui::InvisibleButton("##InvisiblePadd", ImVec2(10.f, 100.f));
    ShowPreview();
    SaveFlipBook();

    if (ImGui::Button("Clear FlipBook List"))
    {
        m_vecSprite.clear();
    }

}
