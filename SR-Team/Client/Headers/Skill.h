#pragma once

#ifndef __SKILL_H__
#define __SKILL_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CSkill final : public CUIObject
{
public:
	enum SKILL_SORT
	{
		ACTIVE, PASSIVE, SKILL_SORT_END
	};

public:
	explicit CSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkill(const CSkill& other);
	virtual ~CSkill() = default;

public:
	_bool Get_Render() { return m_bRender; }

public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_UI() override;

private:
	// ------------------------------------------------------------------
	// 액티브/패시브 스킬 중 하나를 선택하는 함수
	// ------------------------------------------------------------------
	HRESULT Set_SkillSort();
	// ------------------------------------------------------------------
	// 액티브 스킬의 정보를 옆에 보여주는 함수
	// ------------------------------------------------------------------
	HRESULT Show_ActiveSkill_Info();
	// ------------------------------------------------------------------
	// 스킬 아이콘을 퀵슬롯으로 옮기는 함수
	// ------------------------------------------------------------------
	HRESULT Move_To_QuickSlot();

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	
private:
	_bool					m_bRender = false;

// 스킬 윈도우
private:
	// 스킬 윈도우 중심 위치
	_vec3					m_vCenterPos = { 500.f, 300.f, 0.f };
	CTransform*				m_pTransformWnd[SKILL_END];
	CTexture*				m_pTextureWnd[SKILL_END];
	RECT					m_tWndCollRt[SKILL_END];

// 패시브/액티브 스킬 종류
private:
	SKILL_SORT				m_eSkillSort = ACTIVE;	// 스킬 종류
	RECT					m_tSkillSortCollRt[2];	// 액티브/패시브 버튼 충돌 렉트

private:
	RECT					m_tActiveCollRt[6];

private:
	CTexture*				m_pTextureSkillInfo[6] = { nullptr, };
	_int					m_iSkillInfoIndex = -1;
	_bool					m_bRenderSkillInfo = false;
	_vec3					m_vSkillInfoPos = { 0.f, 0.f, 0.f };
};

END
#endif