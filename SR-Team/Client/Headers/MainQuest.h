#pragma once

#ifndef __MAINQUEST_H__
#define __MAINQUEST_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CMainQuest : public CUIObject
{
public:
	enum MAIN_STATE
	{
		MAIN_STATE_NOCLEAR, MAIN_STATE_CLEAR, MAIN_STATE_END
	};
	enum GOLEM_SORT
	{
		RED, GREEN, BLUE, PUPLE, GOLEM_SORT_END
	};

public:
	explicit CMainQuest(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CMainQuest(const CMainQuest& other);
	virtual ~CMainQuest() = default;

public:
	_bool Get_Chart() { return m_bChat; }

public:
	void Set_StartQuest(_bool bStart) { m_bStartQuest = bStart; }

public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_UI() override;

private:
	HRESULT Render_HelpWnd();

private:
	HRESULT Add_Component();

private:
	HRESULT Check_GolemCore_Count();

public:
	static CMainQuest* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture*		m_pTextureWnd[MAINQUEST_END] = { nullptr, };
	eMainQuest_ID	m_eSituation = MAINQUEST_END;

	// HelpWnd
	CTexture*		m_pTextureHelp[MAIN_STATE_END] = { nullptr, };

	_bool			m_bGetGolemCore[GOLEM_SORT_END] = { false, };

	_bool			m_bClear = false;
	_bool			m_bGetReward = false;
	_bool			m_bStartQuest = false;
	_bool			m_bRenderClear = false;

	// NPC와 대화중인지
	_bool				m_bChat = false;
};


END
#endif // !__MAINQUEST_H__
