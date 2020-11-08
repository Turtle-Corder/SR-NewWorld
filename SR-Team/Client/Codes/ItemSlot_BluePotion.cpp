#include "stdafx.h"
#include "..\Headers\ItemSlot_BluePotion.h"
#include "Equip.h"
#include "Inventory.h"
#include "Player.h"

USING(Client)

CItemSlot_BluePotion::CItemSlot_BluePotion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CItemSlot(_pDevice, _pSprite, _pFont)
{
}

CItemSlot_BluePotion::CItemSlot_BluePotion(const CItemSlot_BluePotion & _rOther)
	: CItemSlot(_rOther)
{
}

HRESULT CItemSlot_BluePotion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItemSlot_BluePotion::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 3.f;


	return S_OK;
}

_int CItemSlot_BluePotion::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CItemSlot_BluePotion::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CItemSlot_BluePotion * CItemSlot_BluePotion::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CItemSlot_BluePotion* pInstance = new CItemSlot_BluePotion(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CBluePotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItemSlot_BluePotion::Clone_GameObject(void * _pArg)
{
	CItemSlot_BluePotion* pInstance = new CItemSlot_BluePotion(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CBluePotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CItemSlot_BluePotion::Actual_UseItem()
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
	pPlayerStat->Change_Mp(pPlayerStat->Get_Status().iMp + 30);
	
	// 인벤에서 포션 아이템 개수 감소
	pInven->Use_Potion(BLUE_POTION);

	return true;
}
