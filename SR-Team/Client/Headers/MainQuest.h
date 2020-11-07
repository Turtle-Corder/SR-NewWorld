#pragma once

#ifndef __MAINQUEST_H__
#define __MAINQUEST_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CMainQuest : public CUIObject
{
public:
	explicit CMainQuest(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CMainQuest(const CMainQuest& other);
	virtual ~CMainQuest() = default;

public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_UI() override;

private:
	HRESULT Add_Component();

public:
	static CMainQuest* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture*		m_pTextureWnd[MAINQUEST_END];
	eMainQuest_ID	m_eSituation = MAINQUEST_END;

	_bool			m_bClear = false;
	_bool			m_bGetReward = false;
	_bool			m_bStartQuest = false;
	_bool			m_bRenderClear = false;
};


END
#endif // !__MAINQUEST_H__
