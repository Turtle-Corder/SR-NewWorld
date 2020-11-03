#pragma once

#ifndef __ITEMSLOT_REDELIXER_H__
#define __ITEMSLOT_REDELIXER_H__

#include "ItemSlot.h"
USING(Engine)
BEGIN(Client)

class CItemSlot_RedElixir : public CItemSlot
{
public:
	explicit CItemSlot_RedElixir(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CItemSlot_RedElixir(const CItemSlot_RedElixir& _rOther);
	virtual ~CItemSlot_RedElixir() = default;

public:
	// CPlayerItem을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CItemSlot_RedElixir* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual _bool Actual_UseItem() override;
};

END
#endif