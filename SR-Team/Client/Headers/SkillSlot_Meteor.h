#pragma once

#ifndef __METEOSKILL_H__
#define __METEOSKILL_H__

#include "SkillSlot.h"
USING(Engine)
BEGIN(Client)

class CSkillSlot_Meteor : public CSkillSlot
{
public:
	explicit CSkillSlot_Meteor(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkillSlot_Meteor(const CSkillSlot_Meteor& _rOther);
	virtual ~CSkillSlot_Meteor() = default;

public:
	// CPlayerSkill을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CSkillSlot_Meteor* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual _bool Actual_UseSkill(void* _pArg = nullptr) override;

};

END
#endif