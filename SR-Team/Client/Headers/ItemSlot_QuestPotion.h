#pragma once

#ifndef __ITEMSLOTQUESTPOTION_H__
#define __ITEMSLOTQUESTPOTION_H__

#include "ItemSlot.h"
USING(Engine)
BEGIN(Client)

class CItemSlot_QuestPotion : public CItemSlot
{
public:
	explicit CItemSlot_QuestPotion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CItemSlot_QuestPotion(const CItemSlot_QuestPotion& _rOther);
	virtual ~CItemSlot_QuestPotion() = default;

public:
	// CItemSlot을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CItemSlot_QuestPotion* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	
public:
	virtual _bool Actual_UseItem() override;
};


END
#endif // !__ITEMSLOTQUESTPOTION_H__
