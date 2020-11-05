#pragma once
#ifndef __SKILLSLOT_BLIND_H__
#define __SKILLSLOT_BLIND_H__

#include "SkillSlot.h"
USING(Engine)
BEGIN(Client)

class CSkillSlot_Blind final : public CSkillSlot
{
private:
	explicit CSkillSlot_Blind(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkillSlot_Blind(const CSkillSlot_Blind& _rOther);
	virtual ~CSkillSlot_Blind() = default;

public:
	// CPlayerSkill을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CSkillSlot_Blind* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual _bool Actual_UseSkill(void* _pArg = nullptr) override;

};

END

#endif // !__SKILLSLOT_BLIND_H__
