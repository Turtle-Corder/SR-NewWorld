#include "stdafx.h"
#include "..\Headers\SkillSlotManager.h"

USING(Client)
CSkillSlotManager* CSkillSlotManager::m_pInstance = nullptr;

CSkillSlotManager::CSkillSlotManager()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pSlot_Meteor = (CSkillSlot*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_PlayerSkill", 0);;
	m_pSlot_IceSpear = (CSkillSlot*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_PlayerSkill", 1);
	m_pSlot_ThunderStorm = (CSkillSlot*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_PlayerSkill", 2);
	m_pSlot_ManaDrift = (CSkillSlot*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_PlayerSkill", 3);
	m_pSlot_Blind = (CSkillSlot*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_PlayerSkill", 4);
	m_pSlot_Shield = (CSkillSlot*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_PlayerSkill", 5);
	m_pSlot_IceBlast = (CSkillSlot*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_PlayerSkill", 6);
	m_pSlot_Explosion = (CSkillSlot*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_PlayerSkill", 7);
}

CSkillSlot * CSkillSlotManager::Get_PlayerSkill(eActiveSkill_ID eSkillID)
{
	switch (eSkillID)
	{

	case ACTIVE_ICE_BLAST:
		return m_pSlot_Meteor;

	case ACTIVE_MANA_DRIFT:
		return m_pSlot_IceSpear;
	
	case ACTIVE_ENERGY_EXPLOTIATION:
		return m_pSlot_ThunderStorm;
	
	case ACTIVE_FLAME_WAVE:
		return m_pSlot_ManaDrift;
	
	case ACTIVE_ICE_SPEAR:
		return m_pSlot_Blind;
	
	case ACTIVE_MAGIC_ARMOR:
		return m_pSlot_Shield;
	
	case ACTIVE_THUNDER_STORM:
		return m_pSlot_IceBlast;
	
	case ACTIVE_BLIND:
		return m_pSlot_Explosion;
	
	default:
		break;

	}

	return nullptr;
}
