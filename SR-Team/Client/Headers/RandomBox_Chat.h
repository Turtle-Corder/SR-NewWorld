#pragma once

#ifndef __RANDOMBOXCHAT_H__
#define __RANDOMBOXCHAT_H_

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CRandomBox_Chat : public CUIObject
{
public:
	explicit CRandomBox_Chat(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CRandomBox_Chat(const CRandomBox_Chat& other);
	virtual ~CRandomBox_Chat() = default;

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
	static CRandomBox_Chat* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	_bool				m_bStartChat = false;
};


END
#endif // !__RANDOMBOXCHAT_H__
