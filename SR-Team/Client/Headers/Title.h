#pragma once
#ifndef __TITLE_H__
#define __TITLE_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CTitle final : public CUIObject
{
private:
	explicit CTitle(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CTitle(const CTitle& other);
	virtual ~CTitle() = default;

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
	static CTitle* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture* m_pTexture = nullptr;
	_bool	m_bEnding;
};

END

#endif // !__TITLE_H__

