#include "pch.h"
#include "CRayCaster2D.h"

#include "CTransform.h"
#include "CTileMap.h"
#include "CMeshRender.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CTileMap.h"
#include "CKeyMgr.h"


CRayCaster2D::CRayCaster2D()
	: CComponent(COMPONENT_TYPE::RAYCASTER2D)
	, m_CurrentPos(Vec4())
	, m_CurrentDir(Vec3())
	, m_Ray2DBuffer(nullptr)
	, m_TargetTileMap(nullptr)
	, m_Rays{}
	, m_IsMiniMap(false)
	, m_Fov(60)
	, m_RayAmount(WINDOW_WIDTH)
{
	m_Ray2DBuffer = new CStructuredBuffer;
}

CRayCaster2D::CRayCaster2D(CRayCaster2D& _Origin)
	: CComponent(COMPONENT_TYPE::RAYCASTER2D)
	, m_CurrentPos(_Origin.m_CurrentPos)
	, m_CurrentDir(_Origin.m_CurrentDir)
	, m_Ray2DBuffer(nullptr)
	, m_TargetTileMap(_Origin.m_TargetTileMap)
	, m_Rays{}
	, m_IsMiniMap(false)
	, m_RayAmount(_Origin.m_RayAmount)
	, m_Fov(_Origin.m_Fov)
{
	m_Ray2DBuffer = new CStructuredBuffer;
	SetRayAmount(m_RayAmount);
	SetFOV(m_Fov);
}

CRayCaster2D::~CRayCaster2D()
{
	if (nullptr != m_Ray2DBuffer)
	{
		delete m_Ray2DBuffer;
	}
}

void CRayCaster2D::Init()
{
}

void CRayCaster2D::FinalTick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	m_CurrentPos = Transform()->GetRelativePos();
	m_CurrentDir = Transform()->GetRelativeDir(DIR::UP);
	m_CurrentRot = GetOwner()->Transform()->GetRelativeRotation();

	m_IsMiniMap = false;

	if (KEY_TAP(KEY::M))
		m_IsMiniMap = !m_IsMiniMap;

	if (m_TargetTileMap == nullptr)
	{
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CGameObject* pTarget = pCurLevel->FindObjectByName(L"TileMap");
		pTarget == nullptr ? m_TargetTileMap = nullptr : m_TargetTileMap = pTarget;
	}


	if (m_TargetTileMap && !(m_TargetTileMap->IsDead()))
	{
		vector<tTileInfo> vecTileInfo = m_TargetTileMap->TileMap()->GetTileInfo();
		UINT TileRow = m_TargetTileMap->TileMap()->GetRow();
		UINT TileCol = m_TargetTileMap->TileMap()->GetCol();
		Vec2 vTileSize = m_TargetTileMap->TileMap()->GetTileSize();
		Vec2 vMapSize = { vTileSize.x * TileCol, vTileSize.y * TileRow };

		float start_fov_radian = +deg2rad(m_Fov/2);
		float ray_step_radian = -deg2rad(m_Fov / m_RayAmount);

		Vec2 RayStartDir = Vec2(m_CurrentDir.x * cos(start_fov_radian) - m_CurrentDir.y * sin(start_fov_radian), m_CurrentDir.x * sin(start_fov_radian) + m_CurrentDir.y * cos(start_fov_radian));
		for (int i = 0; i < m_RayAmount; i++)
		{
			m_Rays[i].RayPos = m_CurrentPos;
			m_Rays[i].RayDir = Vec2(RayStartDir.x * cos(ray_step_radian * i) - RayStartDir.y * sin(ray_step_radian * i), RayStartDir.x * sin(ray_step_radian * i) + RayStartDir.y * cos(ray_step_radian * i));
			Vec2 vRayStart = m_Rays[i].RayPos;
			
			Vec2 vRayUnitStepSize = { sqrt(1 + (m_Rays[i].RayDir.y * m_Rays[i].RayDir.y) / (m_Rays[i].RayDir.x * m_Rays[i].RayDir.x)),  sqrt(1 + (m_Rays[i].RayDir.x * m_Rays[i].RayDir.x) / (m_Rays[i].RayDir.y * m_Rays[i].RayDir.y)) };
			Vec2 vMapCheck = vRayStart;
			Vec2 vRayLength1D;
			Vec2 vStep;


			if (m_Rays[i].RayDir.x < 0)
			{
				vStep.x = -1;
				vRayLength1D.x = (vRayStart.x - float(vMapCheck.x)) * vRayUnitStepSize.x;
			}
			else
			{
				vStep.x = 1;
				vRayLength1D.x = (float(vMapCheck.x + 1) - vRayStart.x) * vRayUnitStepSize.x;
			}

			if (m_Rays[i].RayDir.y < 0)
			{
				vStep.y = -1;
				vRayLength1D.y = (vRayStart.y - float(vMapCheck.y)) * vRayUnitStepSize.y;
			}
			else
			{
				vStep.y = 1;
				vRayLength1D.y = (float(vMapCheck.y + 1) - vRayStart.y) * vRayUnitStepSize.y;
			}

			bool bTileFound = false;
			float fMaxDistance = 10000.f;
			float fDistance = 0.f;


			while (!bTileFound && fDistance < fMaxDistance)
			{
				m_Rays[i].imgIdx = 0;

				if (vRayLength1D.x < vRayLength1D.y)
				{
					vMapCheck.x += vStep.x;
					fDistance = vRayLength1D.x;
					vRayLength1D.x += vRayUnitStepSize.x;
					m_Rays[i].HitVert = false;
				}
				else
				{
					vMapCheck.y += vStep.y;
					fDistance = vRayLength1D.y;
					vRayLength1D.y += vRayUnitStepSize.y;
					m_Rays[i].HitVert = true;
				}

				int curCheckCol = vMapCheck.x / vTileSize.x;
				int curCheckRow = abs(vMapCheck.y) / vTileSize.y;

				if (curCheckCol >= 0 && curCheckCol < TileCol && curCheckRow >= 0 && curCheckRow < TileRow)
				{
					if (vecTileInfo[curCheckRow * TileCol + curCheckCol].ImgIdx != 0)
					{
						bTileFound = true;
						m_Rays[i].imgIdx = vecTileInfo[curCheckRow * TileCol + curCheckCol].ImgIdx;
					}
				}

				Vec2 vIntersection;
				Vec2 vDistance;
				if (bTileFound)
				{
					vIntersection = vRayStart + m_Rays[i].RayDir * fDistance;
					m_curIntersection = vIntersection;
					m_Rays[i].RayIntersection = vIntersection;
					m_Rays[i].RayDist = fDistance;
				}


			}

		}

		
	}
	m_Ray2DBuffer->SetData(m_Rays.data(), sizeof(tRay2D) * m_RayAmount);
}


void CRayCaster2D::SetRayAmount(int _Amount)
{
	m_Rays.resize(_Amount);
	m_RayAmount = _Amount;

	// 타일정보를 전달받아서 t 레지스터에 전달시킬 구조화버퍼가 타일 전체 데이터 사이즈보다 작으면 리사이즈
	if (m_Ray2DBuffer->GetElementCount() < m_RayAmount)
	{
		m_Ray2DBuffer->Create(sizeof(tRay2D), m_RayAmount, SB_TYPE::SRV_ONLY, true);
	}
}


void CRayCaster2D::SaveToFile(FILE* _File)
{
	fwrite(&m_CurrentPos, sizeof(Vec4), 1, _File);
	fwrite(&m_CurrentDir, sizeof(Vec3), 1, _File);
	fwrite(&m_CurrentRot, sizeof(Vec3), 1, _File);

	fwrite(&m_RayAmount, sizeof(int), 1, _File);
	fwrite(&m_IsMiniMap, sizeof(bool), 1, _File);
	fwrite(&m_Fov, sizeof(float), 1, _File);
}

void CRayCaster2D::LoadFromFile(FILE* _File)
{
	fread(&m_CurrentPos, sizeof(Vec4), 1, _File);
	fread(&m_CurrentDir, sizeof(Vec3), 1, _File);
	fread(&m_CurrentRot, sizeof(Vec3), 1, _File);

	fread(&m_RayAmount, sizeof(int), 1, _File);
	fread(&m_IsMiniMap, sizeof(bool), 1, _File);
	fread(&m_Fov, sizeof(float), 1, _File);
	
	SetRayAmount(WINDOW_WIDTH);
	SetFOV(m_Fov);
}







