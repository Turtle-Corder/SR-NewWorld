#pragma once

#ifndef __ENERGYEXPLOITATIONSKILL_H__
#define __ENERGYEXPLOITATIONSKILL_H__

#include "SkillSlot.h"
USING(Engine)
BEGIN(Client)

class CSkillSlot_EnergyExplosion : public CSkillSlot
{
public:
	explicit CSkillSlot_EnergyExplosion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkillSlot_EnergyExplosion(const CSkillSlot_EnergyExplosion& _rOther);
	virtual ~CSkillSlot_EnergyExplosion() = default;

public:
	// CPlayerSkill을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CSkillSlot_EnergyExplosion* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual _bool Actual_UseSkill(void* _pArg = nullptr) override;

};

END
#endif