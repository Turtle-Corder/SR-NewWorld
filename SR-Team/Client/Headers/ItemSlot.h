#pragma once

#ifndef __ITEMSLOT_H__
#define __ITEMSLOT_H__

#include "UIObject.h"
BEGIN(Client)
USING(Engine)

class CItemSlot abstract : public CUIObject
{
public:
	explicit CItemSlot(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CItemSlot(const CItemSlot& _rOther);
	virtual ~CItemSlot() = default;

protected:
	HRESULT Update_Delay(_float _fDeltaTime);

public:
	virtual _bool Can_UseItem();
	virtual _bool Actual_UseItem() = 0;
	_float Get_Progress();

public:
	virtual void Free() override;



protected:
	_int		m_iCanUseCnt = 0;
	_int		m_iMaxUseCnt = 0;

	_float		m_fDelayCnt = 0;
	_float		m_fEachDelay = 0;

	_int		m_iConsumeMP = 0;
};

END
#endif