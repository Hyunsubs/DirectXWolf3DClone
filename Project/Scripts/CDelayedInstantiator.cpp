#include "pch.h"
#include "CDelayedInstantiator.h"

CDelayedInstantiator::CDelayedInstantiator()
	: CScript(SCRIPT_TYPE::DELAYEDINSTANTIATOR)
	, m_DelayedTime(0.f)
	, m_SyncTrigger(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "delayed time:", &m_DelayedTime);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "TargetPref:", &m_TargetPref);
}

CDelayedInstantiator::~CDelayedInstantiator()
{

}

void CDelayedInstantiator::Tick()
{
	m_DelayedTime -= DT;
	if (m_DelayedTime <= 0.f && !m_SyncTrigger)
	{
		wstring name = m_TargetPref->GetKey();
		int folder = name.find_last_of(L'\\');
		name.erase(0, folder + 1);
		int ext = name.find_last_of(L'.');
		name.erase(ext, name.size() - 1);
		
		Instantiate(m_TargetPref, 0, Vec3(), name.c_str());
		DeleteObject(GetOwner());
		m_SyncTrigger = true;
	}
}

void CDelayedInstantiator::SaveToFile(FILE* _File)
{
	fwrite(&m_DelayedTime, sizeof(float), 1, _File);
	SaveAssetRef(m_TargetPref, _File);
}

void CDelayedInstantiator::LoadFromFile(FILE* _File)
{
	fread(&m_DelayedTime, sizeof(float), 1, _File);
	LoadAssetRef(m_TargetPref, _File);
}


