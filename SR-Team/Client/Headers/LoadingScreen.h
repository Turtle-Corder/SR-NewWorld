#pragma once
#ifndef __LOADINGSCREEN_H__
#define __LOADINGSCREEN_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CLoadingScreen final : public CUIObject
{
private:
	explicit CLoadingScreen(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CLoadingScreen(const CLoadingScreen& _rOther);
	virtual ~CLoadingScreen() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_UI();

private:
	HRESULT Add_Component();

public:
	static CLoadingScreen* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;


private:
	CTransform* m_pTransformComp			= nullptr;
	CTexture*	m_pTextureComp_Progress		= nullptr;
	CTexture*	m_pTextureComp_Background	= nullptr;

	_uint		m_iCurFrame = 0;
};

END

#endif // !__LOADINGSCREEN_H__
