#pragma once
#ifndef __ICESTRIKESKILL_H__
#define __ICESTRIKESKILL_H__

#include "SkillSlot.h"

USING(Engine)
BEGIN(Client)

class CSkillSlot_IceBlast final : public CSkillSlot
{
private:
	explicit CSkillSlot_IceBlast(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkillSlot_IceBlast(const CSkillSlot_IceBlast& _rOther);
	virtual ~CSkillSlot_IceBlast() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CSkillSlot_IceBlast* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;

public:
	virtual _bool Actual_UseSkill(void* _pArg = nullptr) override;

};

END

#endif
