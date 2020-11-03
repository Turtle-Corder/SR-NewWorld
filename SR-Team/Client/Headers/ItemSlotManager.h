#pragma once

#ifndef __ITEMMANAGER_H__
#define __ITEMMANAGER_H__

BEGIN(Client)
USING(Engine)
class CItemSlot;

class CItemSlotManager
{
private:
	CItemSlotManager();
	virtual ~CItemSlotManager() = default;

public:
	CItemSlot* Get_PlayerItem(ePotion_ID ePotionID);

public:
	static CItemSlotManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CItemSlotManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static CItemSlotManager*	m_pInstance;

	CItemSlot*			m_pRedPotion = nullptr;
	CItemSlot*			m_pBluePotion = nullptr;
	CItemSlot*			m_pRedElixir = nullptr;
	CItemSlot*			m_pBlueElixir = nullptr;
};

END
#endif