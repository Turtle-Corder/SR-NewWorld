#pragma once

#ifndef __SKILLINVEN_H__
#define __SKILLINVEN_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)
class CSkillSlot;

class CSkillInven : public CUIObject
{
public:
	explicit CSkillInven(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkillInven(const CSkillInven& _rOther);
	virtual ~CSkillInven() = default;

public:
	HRESULT Set_SkillIndex(_int iIndex, eActiveSkill_ID eSkillID);
	_bool Can_UseSkill(_int _iIndex);
	_bool Actual_UseSkill(_int _iIndex, void* _pArg = nullptr);
	eActiveSkill_ID Get_SkillID(_int _iIndex);
	_int Get_ConsumeMP(_int _iIndex);
	_float Get_Progress(_int _iIndex);

public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CSkillInven* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

private:
	CSkillSlot*			m_pPlayerSkill[8];
	eActiveSkill_ID		m_pPlayerSkillID[8];
};

END
#endif