#include "pch.h"
#include "CMiscRender.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "components.h"
#include "CStructuredBuffer.h"
#include "CRenderMgr.h"

CMiscRender::CMiscRender()
	: CComponent(COMPONENT_TYPE::MISCRENDER)
	, m_miscObjects{}
	, m_Fov(0.f)
{

}

CMiscRender::~CMiscRender()
{
	if (nullptr != m_RenderObj)
		delete m_RenderObj;
}

void CMiscRender::Init()
{
	m_RenderObj = new CGameObject;
	m_RenderObj->AddComponent(new CTransform);
	m_RenderObj->AddComponent(new CMeshRender);
}

void CMiscRender::FinalTick()
{
	CRayCaster2D* pRayCaster = RayCaster2D();
	if (nullptr != pRayCaster && !(pRayCaster->IsMiniMap()))
	{
		m_CurrentPos = Transform()->GetWorldPos();
		m_Fov = RayCaster2D()->GetFOV();

		FindMiscObjects();
		CheckPassedObjects();

		CRenderMgr::GetInst()->SetRayBuffer(pRayCaster->GetRayBuffer());
	}

	m_miscObjects.clear();
}


void CMiscRender::FindMiscObjects()
{
	vector<CGameObject*> sprites;
	vector<CGameObject*> monsters;
	vector<CGameObject*> pickups;

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pSpriteLayer = nullptr;
	CLayer* pMonsterLayer = nullptr;
	CLayer* pPickupLayer = nullptr;

	for (int i = 0; i < MAX_LAYER; i++)
	{
		wstring curLayerName = pLevel->GetLayer(i)->GetName().c_str();

		if (i == (UINT)LAYER_NAME::SPRITE)
			pSpriteLayer = pLevel->GetLayer(i);
		if (i == (UINT)LAYER_NAME::MONSTER)
			pMonsterLayer = pLevel->GetLayer(i);
		if (i == (UINT)LAYER_NAME::PICKUP)
			pPickupLayer = pLevel->GetLayer(i);
	}
	
	pSpriteLayer == nullptr ? sprites = {} : sprites = pSpriteLayer->GetParentObjects();
	pMonsterLayer == nullptr ? monsters = {} : monsters = pMonsterLayer->GetParentObjects();
	pPickupLayer == nullptr ? pickups = {} : pickups = pPickupLayer->GetParentObjects();

	m_miscObjects.insert(m_miscObjects.end(), sprites.begin(), sprites.end());
	m_miscObjects.insert(m_miscObjects.end(), monsters.begin(), monsters.end());
	m_miscObjects.insert(m_miscObjects.end(), pickups.begin(), pickups.end());
	
}

void CMiscRender::CheckPassedObjects()
{
	for (int i = 0; i < m_miscObjects.size(); i++)
	{
		Vec3 distVec = m_miscObjects[i]->Transform()->GetWorldPos() - m_CurrentPos;
		Vec2 playerDirection = Vec2(Transform()->GetWorldDir(DIR::UP).x, Transform()->GetWorldDir(DIR::UP).y);

		float playerAngle = atan2(playerDirection.y, playerDirection.x);
		float spriteAngle = (playerAngle - atan2(distVec.y, distVec.x));

		if (spriteAngle > PI)
			spriteAngle -= TWO_PI;
		if (spriteAngle < -PI)
			spriteAngle += TWO_PI;

		CMeshRender* TargetMeshRender = m_miscObjects[i]->MeshRender();
		if (nullptr != TargetMeshRender && fabs(spriteAngle) <= FOV_ANGLE / 2)
		{
			float dist = sqrt(pow(abs(distVec.y), 2) + pow(abs(distVec.x), 2));
			tMiscRender objectInfo = { spriteAngle, dist, m_miscObjects[i]};
			CRenderMgr::GetInst()->AddMiscRender(objectInfo);
		}
	}
}



void CMiscRender::SaveToFile(FILE* _File)
{
	fwrite(&m_Fov, sizeof(float), 1, _File);
}


void CMiscRender::LoadFromFile(FILE* _File)
{
	fread(&m_Fov, sizeof(float), 1, _File);
	SetFOV(m_Fov);
}
