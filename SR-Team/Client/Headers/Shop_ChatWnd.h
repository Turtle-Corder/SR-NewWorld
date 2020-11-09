#pragma once

#ifndef __SHOPCHATWND_H__
#define __SHOPCHATWND_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CShop_ChatWnd : public CUIObject
{
public:
	explicit CShop_ChatWnd(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CShop_ChatWnd(const CShop_ChatWnd& other);
	virtual ~CShop_ChatWnd() = default;

public:
	_bool Get_Chart() { return m_bChat; }

public:
	void Set_StartChat(_bool bCheck) { m_bStartChat = bCheck; }

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
	static CShop_ChatWnd* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture*			m_pTextureCom[SHOPCHAT_END] = { nullptr, };
	eShopChatWnd_ID		m_eSituation = SHOPCHAT_END;
	_bool				m_bStartChat = false;

	// NPC와 대화중인지
	_bool				m_bChat = false;
};

END
#endif