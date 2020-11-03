#include "stdafx.h"
#include "..\Headers\SkillSlotManager.h"

USING(Client)
CSkillSlotManager* CSkillSlotManager::m_pInstance = nullptr;

CSkillSlotManager::CSkillSlotManager()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pLaserSkill = (CSkillSlot*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerSkill", 0);
	m_pMeteoSkill = (CSkillSlot*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerSkill", 1);
	m_pManaDriftSkill = (CSkillSlot*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerSkill", 2);
	m_pEnergyExplotiationSkill = (CSkillSlot*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerSkill", 3);
	m_pIceCrystalSkill = (CSkillSlot*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerSkill", 4);
}

CSkillSlot * CSkillSlotManager::Get_PlayerSkill(eActiveSkill_ID eSkillID)
{
	switch (eSkillID)
	{
	case ACTIVE_ICE_STRIKE:
		return m_pIceCrystalSkill;
		break;
	case ACTIVE_MANA_DRIFT:
		return m_pManaDriftSkill;
	case ACTIVE_ENERGY_EXPLOTIATION:
		return m_pEnergyExplotiationSkill;
	case ACTIVE_FLAME_WAVE:
		return m_pMeteoSkill;
	case ACTIVE_ICE_SPEAR:
		return m_pLaserSkill;
	case ACTIVE_MAGIC_ARMOR:
		break;
	case ACTIVE_SKILL_END:
		break;
	default:
		break;
	}

	return nullptr;
}

