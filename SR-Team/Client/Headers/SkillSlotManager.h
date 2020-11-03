#pragma once

#ifndef __SKILLMANAGER_H__
#define __SKILLMANAGER_H__


BEGIN(Client)
USING(Engine)
class CSkillSlot;

class CSkillSlotManager 
{
private:
	CSkillSlotManager();
	virtual ~CSkillSlotManager() = default;

public:
	CSkillSlot* Get_PlayerSkill(eActiveSkill_ID eSkillID);

public:
	static CSkillSlotManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSkillSlotManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static CSkillSlotManager*	m_pInstance;

	CSkillSlot*			m_pLaserSkill = nullptr;
	CSkillSlot*			m_pMeteoSkill = nullptr;
	CSkillSlot*			m_pManaDriftSkill = nullptr;
	CSkillSlot*			m_pEnergyExplotiationSkill = nullptr;
	CSkillSlot*			m_pIceCrystalSkill = nullptr;
};

END
#endif