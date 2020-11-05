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

	CSkillSlot*		m_pSlot_Meteor			= nullptr;
	CSkillSlot*		m_pSlot_IceSpear		= nullptr;
	CSkillSlot*		m_pSlot_ThunderStorm	= nullptr;
	CSkillSlot*		m_pSlot_ManaDrift		= nullptr;
	CSkillSlot*		m_pSlot_Blind			= nullptr;
	CSkillSlot*		m_pSlot_Shield			= nullptr;
	CSkillSlot*		m_pSlot_IceBlast		= nullptr;
	CSkillSlot*		m_pSlot_Explosion		= nullptr;

};

END

#endif