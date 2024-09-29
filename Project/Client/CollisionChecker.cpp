#include "pch.h"
#include "CollisionChecker.h"
#include <Engine/CCollisionMgr.h>

Vec2 CollisionChecker::m_CollisionInput = {};

CollisionChecker::CollisionChecker()
	: m_CollisionList()
{
}

CollisionChecker::~CollisionChecker()
{
}


void CollisionChecker::Update()
{
	AddCollisionCheck();
	ClearCollision();
	for (int i = 0; i < m_CollisionList.size(); i++)
	{
		CCollisionMgr::GetInst()->CollisionCheck((UINT)m_CollisionList[i].x, (UINT)m_CollisionList[i].y);
	}
	ShowCollisionList();
}

void CollisionChecker::AddCollisionCheck()
{
	ImGui::Text("Collision 1 : ");
	ImGui::SameLine();
	ImGui::InputFloat("##Collision1", &m_CollisionInput.x);
	ImGui::Text("Collision 2 : ");
	ImGui::SameLine();
	ImGui::InputFloat("##Collision2", &m_CollisionInput.y);

	if (ImGui::Button("Add"))
	{
		m_CollisionList.push_back(m_CollisionInput);
	}
}

void CollisionChecker::ClearCollision()
{
	if (ImGui::Button("Clear Collision"))
	{
		CCollisionMgr::GetInst()->CollisionCheckClear();
		m_CollisionList.clear();
	}

}

void CollisionChecker::ShowCollisionList()
{
	static char sz_buff1[5] = "";
	static char sz_buff2[5] = "";
	for (int i = 0; i < m_CollisionList.size(); i++)
	{
		sprintf_s(sz_buff1, 5, "%d", (UINT)m_CollisionList[i].x);
		sprintf_s(sz_buff2, 5, "%d", (UINT)m_CollisionList[i].y);
		ImGui::Text(sz_buff1);
		ImGui::SameLine();
		ImGui::Text(" and ");
		ImGui::SameLine();
		ImGui::Text(sz_buff2);
	}
}

