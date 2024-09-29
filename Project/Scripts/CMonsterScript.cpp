#include "pch.h"
#include "CMonsterScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include "CBulletScript.h"

CMonsterScript::CMonsterScript()
	: CScript(SCRIPT_TYPE::MONSTERSCRIPT)
	, m_Hp(0)
	, m_IsAnim(false)
	, m_TargetObject(nullptr)
	, m_Speed(0.f)
	, m_AnimSpeed(0.f)
	, m_CurState(MonsterState::WALK)
	, m_BulletPrefab(nullptr)
	, m_BulletLayer(0)
	, m_IsDead(false)
	, m_AnimTimer(0.f)
	, m_AttackTime(0)
	, m_BulletIntermission(0.f)
	, m_BulletAmmo(0)
	, m_BulletTimer(0.f)
	, m_HitTrigger(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "MosnterHp", &m_Hp); // 몬스터가 가진 체력
	AddScriptParam(SCRIPT_PARAM::INT, "AttacakTime", &m_AttackTime); // 몇회 총알을 날릴지
	AddScriptParam(SCRIPT_PARAM::INT, "Bullet Layer", &m_BulletLayer); // 총알을 어느 레이어에 인스턴스화 할지

	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_Speed); // 이동 스피드
	AddScriptParam(SCRIPT_PARAM::FLOAT, "AnimTime", &m_AnimTime); // 애니메이션 재생을 몇초간 진행할지
	AddScriptParam(SCRIPT_PARAM::FLOAT, "AnimSpeed", &m_AnimSpeed); // 애니메이션 프레임
	AddScriptParam(SCRIPT_PARAM::FLOAT, "BulletIntermission", &m_BulletIntermission); // 총알을 몇초 간격으로 날릴지

	AddScriptParam(SCRIPT_PARAM::PREFAB, "Bullet Prefab", &m_BulletPrefab); // 총알 프리팹

	// Attack 함수 따로 만들어서 애니메이션과는 다르게 총알을 날려줘야 함
	// 총알 날리는 시간도 따로 지정해줘야 할듯
	// m_BulletIntermission 만큼의 간격으로 총알이 날라감

}

CMonsterScript::CMonsterScript(CMonsterScript& _Origin)
	: CScript(SCRIPT_TYPE::MONSTERSCRIPT)
	, m_Hp(_Origin.m_Hp)
	, m_IsAnim(_Origin.m_IsAnim)
	, m_TargetObject(nullptr)
	, m_Speed(_Origin.m_Speed)
	, m_CurState(MonsterState::WALK)
	, m_AnimSpeed(_Origin.m_AnimSpeed)
	, m_AnimTime(_Origin.m_AnimTime)
	, m_AnimTimer(0.f)
	, m_BulletTimer(0.f)
	, m_BulletPrefab(_Origin.m_BulletPrefab)
	, m_BulletLayer(_Origin.m_BulletLayer)
	, m_IsDead(false)
	, m_AttackTime(_Origin.m_AttackTime)
	, m_BulletAmmo(_Origin.m_BulletAmmo)
	, m_BulletIntermission(_Origin.m_BulletIntermission)
	, m_HitTrigger(false)
{

	AddScriptParam(SCRIPT_PARAM::INT, "MosnterHp", &m_Hp); // 몬스터가 가진 체력
	AddScriptParam(SCRIPT_PARAM::INT, "AttacakTime", &m_AttackTime); // 몇회 총알을 날릴지
	AddScriptParam(SCRIPT_PARAM::INT, "Bullet Layer", &m_BulletLayer); // 총알을 어느 레이어에 인스턴스화 할지

	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_Speed); // 이동 스피드
	AddScriptParam(SCRIPT_PARAM::FLOAT, "AnimTime", &m_AnimTime); // 애니메이션 재생을 몇초간 진행할지
	AddScriptParam(SCRIPT_PARAM::FLOAT, "AnimSpeed", &m_AnimSpeed); // 애니메이션 프레임
	AddScriptParam(SCRIPT_PARAM::FLOAT, "BulletIntermission", &m_BulletIntermission); // 총알을 몇초 간격으로 날릴지

	AddScriptParam(SCRIPT_PARAM::PREFAB, "Bullet Prefab", &m_BulletPrefab); // 총알 프리팹
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::Tick()
{
	// 죽은 상태면 그대로 리턴(본작은 시체가 그대로 존재하기 때문에 건드리지 않음)
	// 히틀러의 경우 마지막 장면으로 넘어가게 바꿔줌
	if (m_IsDead)
	{
		if (0 == wcscmp(GetOwner()->GetName().c_str(), L"Hitler"))
		{
			m_dead_acc_time += DT;
		}

		if (m_dead_acc_time >= 1.f && !is_deathcam)
		{
			extern int g_iTargetLevel;
			extern bool g_bLevelTrigger;
			is_deathcam = true;
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			m_TargetObject = pCurLevel->FindObjectByName(L"RayCaster2D");
			DeleteObject(pCurLevel);

			g_iTargetLevel = 6;
			g_bLevelTrigger = true;
		}
		return;
	}

	sound_time -= DT;

	if(!m_IsDead)
		FollowPlayer();

	if (sound_time <= 0.f && m_CurState == MonsterState::ATTACK)
	{
		if (0 == wcscmp(GetOwner()->GetName().c_str(), L"Guard"))
			monster_sound0->Play(1, 1, false);
		if (0 == wcscmp(GetOwner()->GetName().c_str(), L"General"))
			monster_sound1->Play(1, 1, false);
		if (0 == wcscmp(GetOwner()->GetName().c_str(), L"WhiteOfficer"))
			monster_sound2->Play(1, 1, false);
		if (0 == wcscmp(GetOwner()->GetName().c_str(), L"ZombieNazi"))
			monster_sound3->Play(1, 1, false);
		if (0 == wcscmp(GetOwner()->GetName().c_str(), L"HansGrosse"))
			monster_sound4->Play(1, 1, false);
		if (0 == wcscmp(GetOwner()->GetName().c_str(), L"MecahHitler"))
			monster_sound7->Play(1, 1, false);
		if (0 == wcscmp(GetOwner()->GetName().c_str(), L"Hitler"))
			monster_sound6->Play(1, 1, false);

		sound_time = 20.f;
	}

	// 0 보다 크면 시간을 줄여줌
	if (m_BulletTimer > 0.f)
		m_BulletTimer -= DT;

	// 0초보다 크다는 것은 애니메이션이 재생중이라는 의미
	if (m_AnimTimer > 0.f)
	{
		m_AnimTimer -= DT;
	}

	if (m_AnimTimer <= 0.f)
	{
		m_IsAnim = false;
	}

	// 0초 보다 작을순 없음
	m_BulletTimer < 0.f ? m_BulletTimer = 0.f : m_BulletTimer = m_BulletTimer;
	m_AnimTimer < 0.f ? m_AnimTimer = 0.f : m_AnimTimer = m_AnimTimer;

	// 애니메이션 상태가 아닌경우(걷는 애니메이션의 첫장만 보여줘서 멈춰있는 상태 표현)
	if (!m_IsAnim)
		FlipBookComponent()->Play(0, 1.f, true);

	if (m_BulletAmmo > 0 && m_BulletTimer <= 0.f)
		AttackFunc();

	if (m_Hp <= 0)
	{
		m_CurState = MonsterState::DEAD;
	}


	switch (m_CurState)
	{
	case MonsterState::WALK:
		WalkAnim();
		break;
	case MonsterState::ATTACK:
		AttackAnim();
		break;
	case MonsterState::DEAD:
		DeadAnim();
		break;
	default:
		break;
	}
}

void CMonsterScript::FollowPlayer()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_TargetObject = pCurLevel->FindObjectByName(L"RayCaster2D");
	if (m_TargetObject)
	{
		Vec3 TargetPos = m_TargetObject->Transform()->GetWorldPos();
		Vec3 CurrentPos = Transform()->GetWorldPos();
		float dist = custom_distance2d(TargetPos, CurrentPos);

		CGameObject* TileMapInfo = pCurLevel->FindObjectByName(L"TileMap");

		int row = TileMapInfo->TileMap()->GetRow();
		int col = TileMapInfo->TileMap()->GetCol();
		vector<tTileInfo> tileInfo = TileMapInfo->TileMap()->GetTileInfo();
		Vec2 tile_size = TileMapInfo->TileMap()->GetTileSize();

		if (dist <= 350.f)
		{
			m_CurState = MonsterState::ATTACK;
			return;
		}

		else if (dist > 350.f)
		{
			m_CurState = MonsterState::WALK;
		}

		if (dist > 350.f && dist < 800.f)
		{
			int cur_row = abs(CurrentPos.y) / tile_size.y;
			int cur_col = CurrentPos.x / tile_size.x;
			int dest_row = abs(TargetPos.y) / tile_size.y;
			int dest_col = TargetPos.x / tile_size.x;

			vector<Vec2> Path = FindPath(Vec2(cur_col, cur_row), Vec2(dest_col, dest_row), tileInfo, Vec2(col, row));

			if (!Path.empty())
			{
				Vec2 NextPos = Vec2((Path[0].x * 64.f + 32.f), (Path[0].y * -64.f - 32.f));

				if (CurrentPos.x > NextPos.x)
					CurrentPos.x -= m_Speed * DT;
				if (CurrentPos.x < NextPos.x)
					CurrentPos.x += m_Speed * DT;
				if (CurrentPos.y < NextPos.y)
					CurrentPos.y += m_Speed * DT;
				if (CurrentPos.y > NextPos.y)
					CurrentPos.y -= m_Speed * DT;

				if (tileInfo[col * cur_row + cur_col].ImgIdx == 0)
					Transform()->SetRelativePos(CurrentPos);

			}
		}

	}


}

void CMonsterScript::WalkAnim()
{
	if (false == m_IsAnim)
	{
		m_IsAnim = true;
		m_AnimTimer = m_AnimTime;
		FlipBookComponent()->Play(0, m_AnimSpeed, false);
	}
}


void CMonsterScript::AttackFunc()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_BulletAmmo -= 1;

	if (m_TargetObject != nullptr)
	{
		Vec3 TargetPos = m_TargetObject->Transform()->GetWorldPos();
		Vec3 CurrentPos = Transform()->GetWorldPos();
		float degree = atan2(TargetPos.y - CurrentPos.y, TargetPos.x - CurrentPos.x);
		Vec2 dir = Vec2(cos(degree), sin(degree));

		CGameObject* newBullet = m_BulletPrefab->Instantiate();
		newBullet->Transform()->SetRelativePos(Transform()->GetWorldPos());
		CBulletScript* bulletScript = nullptr;
		vector<CScript*> vec_scripts = newBullet->GetScripts();
		for (int i = 0; i < vec_scripts.size(); i++)
		{
			if (vec_scripts[i]->GetScriptType() == SCRIPT_TYPE::BULLETSCRIPT)
			{
				bulletScript = (CBulletScript*)vec_scripts[i];
			}
		}
		if (bulletScript)
		{
			bulletScript->SetSpeed(1000.f);
			bulletScript->SetDirection(dir);
			attacking_sound->Play(1, 1, false);
			pCurLevel->AddObject((UINT)LAYER_NAME::MONSTER_BULLET, newBullet); // 지정한 레이어로 인스턴스
		}

	}

}

void CMonsterScript::AttackAnim()
{
	if (false == m_IsAnim && m_AnimTimer <= 0.f)
	{
		m_IsAnim = true;
		m_AnimTimer = m_AnimTime;
		m_BulletAmmo = m_AttackTime;
		FlipBookComponent()->Play(1, m_AnimSpeed, false);
	}
	
}

void CMonsterScript::DeadAnim()
{
	if (!m_IsDead)
	{
		FlipBookComponent()->Play(2, m_AnimSpeed, false);
		m_IsAnim = true;
		m_IsDead = true;
		dead_sound->Play(1, 1, true);
	}


}

void CMonsterScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_BulletPrefab, _File);
	fwrite(&m_Hp, sizeof(int), 1, _File);
	fwrite(&m_Speed, sizeof(int), 1, _File);
	fwrite(&m_AnimTime, sizeof(float), 1, _File);
	fwrite(&m_AttackTime, sizeof(int), 1, _File);
	fwrite(&m_AnimSpeed, sizeof(float), 1, _File);
	fwrite(&m_BulletLayer, sizeof(int), 1, _File);
	fwrite(&m_BulletAmmo, sizeof(int), 1, _File);
	fwrite(&m_BulletIntermission, sizeof(float), 1, _File);
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_BulletPrefab, _File);
	fread(&m_Hp, sizeof(int), 1, _File);
	fread(&m_Speed, sizeof(int), 1, _File);
	fread(&m_AnimTime, sizeof(float), 1, _File);
	fread(&m_AttackTime, sizeof(int), 1, _File);
	fread(&m_AnimSpeed, sizeof(float), 1, _File);
	fread(&m_BulletLayer, sizeof(int), 1, _File);
	fread(&m_BulletAmmo, sizeof(int), 1, _File);
	fread(&m_BulletIntermission, sizeof(float), 1, _File);
}

void CMonsterScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if ((UINT)LAYER_NAME::PLAYER_BULLET == _OtherObject->GetLayerIdx() && !m_IsDead)
	{
		m_Hp -= 1;
		attacked_sound->Play(1, 1.f, true);
		DeleteObject(_OtherObject);
	}	

}





