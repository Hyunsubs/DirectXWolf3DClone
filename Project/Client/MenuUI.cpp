#include "pch.h"
#include "MenuUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Scripts/CScriptMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CScript.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CDevice.h>

#include "CEditorMgr.h"
#include "Inspector.h"
#include "ListUI.h"
#include "CLevelSaveLoad.h"
#include "Outliner.h"


MenuUI::MenuUI()
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Tick()
{
    if (!IsActive())
        return;

    if (ImGui::BeginMainMenuBar())
    {

        Update();

        ImGui::EndMainMenuBar();
    }
}


void MenuUI::Update()
{
	File();

	Level();

	GameObject();

	Assets();	

	Tools();

	Layers();
}

void MenuUI::File()
{
	wstring contentPath = CPathMgr::GetInst()->GetContentPath();
	contentPath += L"level\\";

	if (ImGui::BeginMenu("Levels"))
	{
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		ImGui::BeginDisabled(LEVEL_STATE::STOP != pCurLevel->GetState());
		if (ImGui::MenuItem("Level Save"))
		{
			wstring targetPath = FileSaveSystem(L"lv", contentPath);
			if(L"" != targetPath)
			{
				if (nullptr != pCurLevel)
					CLevelSaveLoad::SaveLevel(targetPath, pCurLevel);
			}
		}
		ImGui::EndDisabled();

		if (ImGui::MenuItem("Level Load"))
		{
			wstring targetPath = FileLoadSystem(L"lv", contentPath);
			if (L"" != targetPath)
			{
				ChangeLevelState(LEVEL_STATE::STOP);
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
				pInspector->SetTargetObject(nullptr);
				CLevel* pTargetLevel = CLevelSaveLoad::LoadLevel(targetPath);
				if (nullptr != pTargetLevel)
					ChangeLevel(pTargetLevel, LEVEL_STATE::STOP);
			}
		}

		ImGui::BeginDisabled(LEVEL_STATE::STOP != pCurLevel->GetState());
		if (ImGui::MenuItem("New Level"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);
			CLevel* newLevel = new CLevel;
			ChangeLevel(newLevel, LEVEL_STATE::STOP);
		}
		ImGui::EndDisabled();


		ImGui::EndMenu();
	}
}

void MenuUI::Level()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	ImGui::BeginDisabled(!pCurLevel);

	if (ImGui::BeginMenu("Level"))
	{
		LEVEL_STATE State = LEVEL_STATE::STOP;
		if (nullptr != pCurLevel)
			State = pCurLevel->GetState();
		bool bCamExist = (pCurLevel->FindObjectByName(L"MainCamera") != nullptr);

		ImGui::BeginDisabled(LEVEL_STATE::PLAY == State && !bCamExist);
		if (ImGui::MenuItem("Play"))
		{	
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);

			if (LEVEL_STATE::STOP == State)
			{
				wstring strLevelPath = CPathMgr::GetInst()->GetContentPath();
				strLevelPath += L"levelTemp\\Temp.lv";
				CLevelSaveLoad::SaveLevel(strLevelPath, pCurLevel);
			}
			
			ChangeLevelState(LEVEL_STATE::PLAY);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY != State);
		if (ImGui::MenuItem("Pause"))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::STOP == State);
		if (ImGui::MenuItem("Stop"))
		{
			wstring StrLevelLoadPath = CPathMgr::GetInst()->GetContentPath();
			StrLevelLoadPath += L"levelTemp\\Temp.lv";
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(StrLevelLoadPath);
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

			// Inspector Clear 하기 (이전 오브젝트 정보를 보여주고 있을 수가 있기 때문에)				
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);
		}
		ImGui::EndDisabled();

		ImGui::EndMenu();
	}

	ImGui::EndDisabled();
}

void MenuUI::GameObject()
{
	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	CGameObject* pTargetObj = pInspector->GetTargetObject();

	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object"))
		{
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CGameObject* newObj = new CGameObject;
			newObj->SetName(L"EmptyObj");
			newObj->AddComponent(new CTransform);
			pCurLevel->AddObject(0, newObj);
		}

		if (ImGui::BeginMenu("Add Component"))
		{
			// 인스펙터에 대상 오브젝트가 있을때만
			ImGui::BeginDisabled(nullptr == pTargetObj);

			if (ImGui::MenuItem("Collider2D"))
				pTargetObj->AddComponent(new CCollider2D);

			if (ImGui::MenuItem("Camera"))
			{
				CCamera* pCamera = new CCamera;
				pCamera->SetLayerAll();
				pCamera->SetPriority(0);
				pTargetObj->AddComponent(pCamera);
				CRenderMgr::GetInst()->RegisterCamera(pTargetObj->Camera(), 0);
			}

			if (ImGui::MenuItem("Light2D"))
				pTargetObj->AddComponent(new CLight2D);

			if (ImGui::MenuItem("FlipBook"))
				pTargetObj->AddComponent(new CFlipBookComponent);

			if (ImGui::MenuItem("MiscRender"))
				pTargetObj->AddComponent(new CMiscRender);
			
			ImGui::EndDisabled();

			ImGui::BeginDisabled(!(nullptr != pTargetObj &&
				nullptr == pTargetObj->TileMap() &&
				nullptr == pTargetObj->MeshRender() &&
				nullptr == pTargetObj->ParticleSystem() &&
				nullptr == pTargetObj->RayCaster2D()));

			// 렌더 컴포넌트 중복추가 방지
			if (ImGui::MenuItem("MeshRender"))
				pTargetObj->AddComponent(new CMeshRender);
			if (ImGui::MenuItem("ParticleSystem"))
				pTargetObj->AddComponent(new CParticleSystem);
			if (ImGui::MenuItem("RayCaster2D"))
				pTargetObj->AddComponent(new CRayCaster2D);

			ImGui::EndDisabled();

			ImGui::EndMenu();


		}

		if (ImGui::BeginMenu("TileMap Object"))
		{
			if (ImGui::MenuItem("Tile Load From Pref"))
			{
				wstring contentPath = CPathMgr::GetInst()->GetContentPath();
				contentPath += L"prefab\\TileMaps\\";
				wstring TargetPath = FileLoadSystem(L"pref", contentPath);
				if(L"" != TargetPath)
				{
					Ptr<CPrefab> pPref = new CPrefab;
					pPref->Load(TargetPath);
					CGameObject* protoObj = pPref->Instantiate();
					protoObj->SetName(L"TileMap");
					protoObj->TileMap()->SetRowCol(protoObj->TileMap()->GetRow(), protoObj->TileMap()->GetCol());
					CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
					pCurLevel->AddObject(0, protoObj);
				}
			}
			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("Prefab(None Tile)"))
		{
			ImGui::BeginDisabled(nullptr == pTargetObj);
			if (ImGui::MenuItem("Make Prefab"))
			{
				wstring contentPath = CPathMgr::GetInst()->GetContentPath();
				contentPath += L"prefab\\";
				wstring TargetPath = FileSaveSystem(L"pref", contentPath);
				if(L"" != TargetPath)
				{
					Ptr<CPrefab> pPref = new CPrefab;
					pPref->SetProtoObject(pTargetObj->Clone());
					pPref->Save(TargetPath);
				}
			}
			ImGui::EndDisabled();

			if (ImGui::MenuItem("Load Prefab"))
			{
				wstring contentPath = CPathMgr::GetInst()->GetContentPath();
				contentPath += L"prefab\\";
				wstring TargetPath = FileLoadSystem(L"pref", contentPath);
				if(L"" != TargetPath)
				{
					Ptr<CPrefab> pPref = new CPrefab;
					pPref->Load(TargetPath);
					CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
					CGameObject* clonedObj = pPref->Instantiate();
					pCurLevel->AddObject(0, clonedObj);
				}
			}

			ImGui::EndMenu();
		}

		AddScript();

		ImGui::EndMenu();
	}
}

void MenuUI::AddScript()
{
	if (ImGui::BeginMenu("Add Script"))
	{
		vector<wstring> vecScriptsName;
		CScriptMgr::GetScriptInfo(vecScriptsName);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{	
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				//CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);

				// 인스펙터
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

				// 타겟 오브젝트 알아냄
				CGameObject* pObject = pInspector->GetTargetObject();
				if (nullptr != pObject)
				{
					// 오브젝트에, 선택한 스크립트를 추가해줌
					CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
					vector<CScript*> vecScripts = pObject->GetScripts();
					bool isExist = false;
					for (int i = 0; i < vecScripts.size(); i++)
					{
						if (pScript->GetScriptType() == vecScripts[i]->GetScriptType())
							isExist = true;
					}
					if(!isExist)
						pObject->AddComponent(pScript);
				}
			}
		}

		ImGui::EndMenu();
	}

}


void MenuUI::Assets()
{
	if (ImGui::BeginMenu("Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<CMaterial> pMtrl = new CMaterial;						
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			CAssetMgr::GetInst()->AddAsset<CMaterial>(Key, pMtrl);
			pMtrl->Save(Key);
		}



		EditorUI* pSpriteEditor = CEditorMgr::GetInst()->FindEditorUI("SpriteEditor");				
		bool IsActive = pSpriteEditor->IsActive();

		if (ImGui::MenuItem("Sprite Editor", nullptr, &IsActive))
		{			
			CEditorMgr::GetInst()->FindEditorUI("SpriteEditor")->SetActive(IsActive);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Tools()
{
	if (ImGui::BeginMenu("Tools"))
	{
		if (ImGui::MenuItem("TileMapTool"))
		{
			EditorUI* pUI = CEditorMgr::GetInst()->FindEditorUI("TileMapTool");

			if (pUI->IsActive())
			{
				pUI->SetActive(false);
			}
			else
			{
				pUI->SetActive(true);
			}
		}

		if (ImGui::MenuItem("FlipBookTool"))
		{
			EditorUI* pUI = CEditorMgr::GetInst()->FindEditorUI("FlipBookTool");

			if (pUI->IsActive())
			{
				pUI->SetActive(false);
			}
			else
			{
				pUI->SetActive(true);
			}
		}

		if (ImGui::MenuItem("SpriteEditor"))
		{
			EditorUI* pUI = CEditorMgr::GetInst()->FindEditorUI("SpriteEditor");

			if (pUI->IsActive())
			{
				pUI->SetActive(false);
			}
			else
			{
				pUI->SetActive(true);
			}
		}

		if (ImGui::MenuItem("CollisionChecker"))
		{
			EditorUI* pUI = CEditorMgr::GetInst()->FindEditorUI("CollisionChecker");

			if (pUI->IsActive())
			{
				pUI->SetActive(false);
			}
			else
			{
				pUI->SetActive(true);
			}
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Layers()
{
	if (ImGui::BeginMenu("Layer Names"))
	{
		EditorUI* pUI = CEditorMgr::GetInst()->FindEditorUI("LayerUI");
		pUI->IsActive() ? pUI->SetActive(false) : pUI->SetActive(true);

		ImGui::EndMenu();
	}
}



wstring MenuUI::GetAssetKey(ASSET_TYPE _Type, const wstring& _KeyFormat)
{
	wstring Key;

	switch (_Type)
	{	
	case ASSET_TYPE::MATERIAL:
	{
		Key = wstring(L"material\\") + _KeyFormat + L" %d.mtrl";
	}	
		break;
	case ASSET_TYPE::PREFAB:
	{
		Key = wstring(L"prefab\\") + _KeyFormat + L" %d.pref";
	}
		break;
	case ASSET_TYPE::SPRITE:
	{
		Key = wstring(L"sprite\\") + _KeyFormat + L" %d.sprite";
	}
		break;
	case ASSET_TYPE::FLIPBOOK:
	{
		Key = wstring(L"flipbook\\") + _KeyFormat + L" %d.flip";
	}
		break;	
	break;
	default:
		assert(nullptr);
		break;
	}

	wchar_t szKey[255] = {};
	wstring FilePath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < 0xffffffff; ++i)
	{		
		swprintf_s(szKey, 255, Key.c_str(), i);
		
		if (false == std::filesystem::exists(FilePath + szKey))
		{
			break;
		}
	}

	return szKey;
}
