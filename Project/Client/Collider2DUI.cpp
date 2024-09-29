#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>


Collider2DUI::Collider2DUI()
	: ComponentUI(COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Update()
{
	Title();

	CCollider2D* pCollider = GetTargetObject()->Collider2D();
	
	// Offset
	Vec3 Offset = pCollider->GetOffset();
	ImGui::Text("Offset:");
	ImGui::SameLine();
	ImGui::InputFloat3("##ColliderOffset", Offset);

	// Scale
	Vec3 Scale = pCollider->GetScale();
	ImGui::Text("Scale:");
	ImGui::SameLine();
	ImGui::InputFloat3("##ColliderScale", Scale);

	bool Independent = pCollider->GetIndependentScale();
	// Independent Scale CheckBox
	ImGui::Checkbox("IndepedentScale", &Independent);

	pCollider->SetOffset(Offset);
	pCollider->SetScale(Scale);
	pCollider->SetIndependentScale(Independent);
}