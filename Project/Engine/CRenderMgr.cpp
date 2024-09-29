#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CCamera.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CLight2D.h"
#include "CStructuredBuffer.h"
#include "CFlipBookComponent.h"
#include "CFontMgr.h"
#include "CRayCaster2D.h"
#include "CTileMap.h"

#include "CKeyMgr.h"

CRenderMgr::CRenderMgr()
	: m_EditorCamera(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_vecMiscRender{}
	, m_RayBuffer(nullptr)
{
	m_Light2DBuffer = new CStructuredBuffer;
}

CRenderMgr::~CRenderMgr()
{
	DELETE(m_DebugObject);
	DELETE(m_Light2DBuffer);
}

void CRenderMgr::Init()
{
	// AssetMgr 가 초기화될때 만들어둔 후처리용 텍스쳐를 참조한다.
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");


	// 디버그 렌더링용 게임 오브젝트
	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(FINDMATERIAL(L"DebugShapeMtrl"));
}

void CRenderMgr::Tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	RenderStart();

	// Level 이 Player 상태인 경우, Level 내에 있는 카메라 시점으로 렌더링하기
	if (PLAY == pCurLevel->GetState())
	{
		for (size_t i = 0; i < m_vecCam.size(); ++i)
		{
			if (nullptr == m_vecCam[i])
				continue;

			m_vecCam[i]->Render();
		}
	}

	// Level 이 Stop 이나 Pause 인 경우, Editor 용 카메라 시점으로 렌더링 하기
	else
	{
		if (nullptr != m_EditorCamera)
		{
			m_EditorCamera->Render();
		}		
	}	



	// Debug Render
#ifdef _DEBUG
	RenderDebugShape();
#endif

	CFontMgr::GetInst()->Render();

	// Clear
	Clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _CamPriority)
{
	// 카메라 우선순위에 따라서 벡터의 공간의 마련되어야 한다.
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	// 카메라 우선순위에 맞는 위치에 넣는다
	m_vecCam[_CamPriority] = _Cam;
}

void CRenderMgr::SortMiscObjects()
{
	for (int i = 0; i < m_vecMiscRender.size(); i++)
	{
		for (int j = 0; j < m_vecMiscRender.size() - 1; j++)
		{
			if (m_vecMiscRender[j].dist < m_vecMiscRender[j + 1].dist) 
			{
				tMiscRender temp = m_vecMiscRender[j + 1];
				m_vecMiscRender[j + 1] = m_vecMiscRender[j];
				m_vecMiscRender[j] = temp;
			}
		}
	}
}


void CRenderMgr::PostProcessCopy()
{
	// RenderTarget -> PostProcessTex
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::RenderStart()
{
	// 렌더타겟 지정
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// TargetClear
	float color[4] = { 0.f, 0.f, 0.f, 1.f };
	CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), color);
	CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	g_GlobalData.g_Resolution = Vec2((float)pRTTex->Width(), (float)pRTTex->Height());
	g_GlobalData.g_Light2DCount = (int)m_vecLight2D.size();
		
	// Light2D 정보 업데이트 및 바인딩
	vector<tLightInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data());
		m_Light2DBuffer->Binding(11);
	}



	// GlobalData 바인딩
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_GlobalData);
	pGlobalCB->Binding();
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
	m_vecMiscRender.clear();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* Tile = pCurLevel->FindObjectByName(L"TileMap");
	CGameObject* RayCaster = pCurLevel->FindObjectByName(L"RayCaster2D");
	
	if (RayCaster)
	{
		RayCaster->RayCaster2D()->GetRayBuffer()->Clear(18);
	}

	if (Tile)
	{
		Tile->TileMap()->GetTileBuffer()->Clear(19);
	}

	CFontMgr::GetInst()->Clear();
}

void CRenderMgr::RenderDebugShape()
{
	list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();

	for (; iter != m_DebugShapeList.end(); )
	{
		// 디버그 요청 모양에 맞는 메시를 가져옴
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));			
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::LINE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"LineMesh_Debug"));
			break;
		case DEBUG_SHAPE::CUBE:

			break;
		case DEBUG_SHAPE::SPHERE:

			break;		
		}
		
		// 위치 세팅
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// 재질 세팅
		m_DebugObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		// 깊이판정 여부에 따라서, 쉐이더의 깊이판정 방식을 결정한다.
		if ((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// 렌더링
		m_DebugObject->MeshRender()->Render();


		// 수명이 다한 디버그 정보를 삭제
		(*iter).Age += EngineDT;
		if ((*iter).LifeTime < (*iter).Age)
		{
			iter = m_DebugShapeList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::RenderMisc()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (LEVEL_STATE::PLAY == pCurLevel->GetState())
	{
		for (int i = 0; i < m_vecMiscRender.size(); i++)
		{
			Vec3 OriginPos = m_vecMiscRender[i].TargetObject->Transform()->GetWorldPos();
			Vec3 OriginRot = m_vecMiscRender[i].TargetObject->Transform()->GetRelativeRotation();
			Vec3 OriginScale = m_vecMiscRender[i].TargetObject->Transform()->GetRelativeScale();
			Matrix OriginMatrix = m_vecMiscRender[i].TargetObject->Transform()->GetWorldMat();

			Vec3 CameraPos = m_vecCam[0]->Transform()->GetWorldPos();
			float spriteHeight = (TILE_SIZE / m_vecMiscRender[i].dist) * DIST_PROJ_PLANE * 1.2f;
			float spriteX = (WINDOW_WIDTH / 2) * (m_vecMiscRender[i].angle / (FOV_ANGLE / 2)); // Fov를 -1 ~ 1로 NDC 좌표계에 맞게 정규화 한 후 윈도우 해상도를 곱하여 화면에서의 월드 좌표를 구해줌

			// 카메라 위치를 미리 더해주어 view변환을 거쳐도 자신의 위치를 그대로 사용
			Vec3 spriteCoord = Vec3(spriteX + CameraPos.x, -10.f + CameraPos.y, 200.f);
			m_vecMiscRender[i].TargetObject->Transform()->SetRelativePos(spriteCoord);
			m_vecMiscRender[i].TargetObject->Transform()->SetRelativeScale(Vec3(spriteHeight, spriteHeight, 1.f));

			Vec3 objScale = m_vecMiscRender[i].TargetObject->Transform()->GetRelativeScale();
			Vec3 objRot = m_vecMiscRender[i].TargetObject->Transform()->GetRelativeRotation();
			Vec3 objPos = m_vecMiscRender[i].TargetObject->Transform()->GetRelativePos();
			Matrix matWorld = XMMatrixScaling(objScale.x, objScale.y, objScale.z)
				* XMMatrixRotationX(objRot.x)
				* XMMatrixRotationY(objRot.y)
				* XMMatrixRotationZ(objRot.z)
				* XMMatrixTranslation(objPos.x, objPos.y, objPos.z);
			m_vecMiscRender[i].TargetObject->Transform()->SetWorldMatrix(matWorld);

			if (nullptr != m_RayBuffer)
				m_RayBuffer->Binding(18);

			m_vecMiscRender[i].TargetObject->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, m_vecMiscRender[i].dist);
			m_vecMiscRender[i].TargetObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
			m_vecMiscRender[i].TargetObject->MeshRender()->GetMaterial()->GetShader()->SetBSType(BS_TYPE::DEFAULT);
			m_vecMiscRender[i].TargetObject->MeshRender()->Render();


			// 렌더 마무리 된 후 다시 원래 위치로 되돌림
			m_vecMiscRender[i].TargetObject->Transform()->SetRelativePos(OriginPos);
			m_vecMiscRender[i].TargetObject->Transform()->SetRelativeScale(OriginScale);
			m_vecMiscRender[i].TargetObject->Transform()->SetRelativeRotation(OriginRot);
			m_vecMiscRender[i].TargetObject->Transform()->SetWorldMatrix(OriginMatrix);
			m_vecMiscRender[i].TargetObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
		}
	}
}

