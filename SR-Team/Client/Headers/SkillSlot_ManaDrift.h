#pragma once

#ifndef __MANADRIFTSKILL_H__
#define __MANADRIFTSKILL_H__

#include "SkillSlot.h"
USING(Engine)
BEGIN(Client)

class CSkillSlot_ManaDrift : public CSkillSlot
{
public:
	explicit CSkillSlot_ManaDrift(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkillSlot_ManaDrift(const CSkillSlot_ManaDrift& _rOther);
	virtual ~CSkillSlot_ManaDrift() = default;

public:
	// CPlayerSkill을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CSkillSlot_ManaDrift* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual _bool Actual_UseSkill(void* _pArg = nullptr) override;
};

END
#endif
