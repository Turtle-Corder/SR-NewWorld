#include "stdafx.h"
#include "..\Headers\ItemSlot_RedPotion.h"
#include "Equip.h"
#include "Inventory.h"

USING(Client)


CItemSlot_RedPotion::CItemSlot_RedPotion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CItemSlot(_pDevice, _pSprite, _pFont)
{
}

CItemSlot_RedPotion::CItemSlot_RedPotion(const CItemSlot_RedPotion & _rOther)
	: CItemSlot(_rOther)
{
}

HRESULT CItemSlot_RedPotion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItemSlot_RedPotion::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;

	m_fEachDelay = 3.f;

	return S_OK;
}

_int CItemSlot_RedPotion::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CItemSlot_RedPotion::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}


CItemSlot_RedPotion * CItemSlot_RedPotion::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CItemSlot_RedPotion* pInstance = new CItemSlot_RedPotion(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CRedPotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItemSlot_RedPotion::Clone_GameObject(void * _pArg)
{
	CItemSlot_RedPotion* pInstance = new CItemSlot_RedPotion(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CRedPotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CItemSlot_RedPotion::Actual_UseItem()
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

	// 장비창에서 플레이어 HP 증가
	pEquip->Set_PlayerHp(30);

	// 인벤에서 포션 아이템 개수 감소
	pInven->Use_Potion(RED_POTION);

	return true;
}
