#pragma once
#ifndef __SKILLSLOT_ICESPEAR_H__
#define __SKILLSLOT_ICESPEAR_H__

#include "SkillSlot.h"
USING(Engine)
BEGIN(Client)

class CSkillSlot_IceSpear final : public CSkillSlot
{
private:
	explicit CSkillSlot_IceSpear(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkillSlot_IceSpear(const CSkillSlot_IceSpear& _rOther);
	virtual ~CSkillSlot_IceSpear() = default;

public:
	// CPlayerSkill을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CSkillSlot_IceSpear* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual _bool Actual_UseSkill(void* _pArg = nullptr) override;
};

END

#endif // !__ICESPEAR_H__
