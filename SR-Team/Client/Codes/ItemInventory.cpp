#include "stdafx.h"
#include "..\Headers\ItemInventory.h"
#include "ItemSlot.h"
#include "ItemSlotManager.h"
USING(Client)

CItemInventory::CItemInventory(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_int i = 0; i < 4; i++)
	{
		m_pPlayerItem[i] = nullptr;
	}
}

CItemInventory::CItemInventory(const CItemInventory & _rOther)
	: CUIObject(_rOther)
{
}

CItemInventory::~CItemInventory()
{
}

HRESULT CItemInventory::Set_ItemIndex(_int iIndex, ePotion_ID ePOTION)
{
	m_pPlayerItem[iIndex] = CItemSlotManager::Get_Instance()->Get_PlayerItem(ePOTION);
	return S_OK;
}

_bool CItemInventory::Can_UseItem(_int _iIndex)
{
	if (m_pPlayerItem[_iIndex])
		return m_pPlayerItem[_iIndex]->Can_UseItem();

	return false;
}

_bool CItemInventory::Actual_UseItem(_int iIndex)
{
	if (m_pPlayerItem[iIndex])
		return m_pPlayerItem[iIndex]->Actual_UseItem();

	return false;
}

HRESULT CItemInventory::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItemInventory::Setup_GameObject(void * _pArg)
{
	return S_OK;
}

_int CItemInventory::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CItemInventory::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CItemInventory * CItemInventory::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CItemInventory* pInstance = new CItemInventory(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CItemInventory", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItemInventory::Clone_GameObject(void * _pArg)
{
	CItemInventory* pInstance = new CItemInventory(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CItemInventory", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
