#pragma once

#ifndef __ITEMINVENTORY_H__
#define __ITEMINVENTORY_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)
class CItemSlot;

class CItemInventory : public CUIObject
{
public:
	explicit CItemInventory(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CItemInventory(const CItemInventory& _rOther);
	virtual ~CItemInventory();

public:
	HRESULT Set_ItemIndex(_int iIndex, ePotion_ID ePOTION);
	_bool Can_UseItem(_int _iIndex);
	_bool Actual_UseItem(_int iIndex);
	_float Get_Progress(_int _iIndex);


public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CItemInventory* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

private:
	CItemSlot*		m_pPlayerItem[4] = { nullptr, };
};

END
#endif