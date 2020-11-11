#include "stdafx.h"
#include "..\Headers\ItemSlotManager.h"


USING(Client)
CItemSlotManager* CItemSlotManager::m_pInstance = nullptr;

CItemSlotManager::CItemSlotManager()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pRedPotion = (CItemSlot*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_PlayerItem", 0);
	m_pBluePotion = (CItemSlot*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_PlayerItem", 1);
	m_pRedElixir = (CItemSlot*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_PlayerItem", 2);
	m_pBlueElixir = (CItemSlot*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_PlayerItem", 3);
}

CItemSlot * CItemSlotManager::Get_PlayerItem(ePotion_ID ePotionID)
{
	switch (ePotionID)
	{
	case RED_POTION:
		return m_pRedPotion;
	case RED_ELIXIR:
		return m_pRedElixir;
	case BLUE_POTION:
		return m_pBluePotion;
	case BLUE_ELIXIR:
		return m_pBlueElixir;
	default:
		break;
	}

	return nullptr;
}
