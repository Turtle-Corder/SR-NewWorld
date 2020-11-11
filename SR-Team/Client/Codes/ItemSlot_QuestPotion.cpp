#include "stdafx.h"
#include "..\Headers\ItemSlot_QuestPotion.h"
#include "Equip.h"
#include "Inventory.h"
#include "Player.h"
#include "Sound_Manager.h"

USING(Client)

CItemSlot_QuestPotion::CItemSlot_QuestPotion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CItemSlot(_pDevice, _pSprite, _pFont)
{
}

CItemSlot_QuestPotion::CItemSlot_QuestPotion(const CItemSlot_QuestPotion & _rOther)
	: CItemSlot(_rOther)
{
}

HRESULT CItemSlot_QuestPotion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItemSlot_QuestPotion::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;

	m_fEachDelay = 3.f;

	return S_OK;
}

_int CItemSlot_QuestPotion::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CItemSlot_QuestPotion::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CItemSlot_QuestPotion * CItemSlot_QuestPotion::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CItemSlot_QuestPotion* pInstance = new CItemSlot_QuestPotion(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CItemSlot_QuestPotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItemSlot_QuestPotion::Clone_GameObject(void * _pArg)
{
	CItemSlot_QuestPotion* pInstance = new CItemSlot_QuestPotion(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CItemSlot_QuestPotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CItemSlot_QuestPotion::Actual_UseItem()
{
	if (!Can_UseItem())
		return false;

	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return false;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return false;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory", 0);
	if (pInven == nullptr)
		return false;

	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player", 0);
	if (pPlayer == nullptr)
		return false;
	CStatus* pPlayerStat = (CStatus*)pManagement->Get_Component(
		pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Stat");
	if (pPlayerStat == nullptr)
		return false;

	// 장비창에서 플레이어 HP 증가
	pPlayerStat->Change_Hp(pPlayerStat->Get_Status().iHp + 100);
	pPlayerStat->Change_Mp(pPlayerStat->Get_Status().iMp + 100);
	//pEquip->Set_PlayerHp(30);

	// 인벤에서 포션 아이템 개수 감소
	pInven->Use_Potion(QUEST_POTION);

	// 사운드
	CSoundManager::Get_Instance()->PlayUI(L"PotionDrug.wav");

	return true;
}
