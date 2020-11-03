#pragma once
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CMouse final : public CUIObject
{
public:
	enum STATE 
	{ 
		IDLE,	// None
		GRAB,	// None
		HELP,	// None

		ANIMSTATE_START,

		CLICK = ANIMSTATE_START,
		WORK,
		STATE_END 
	};

	enum KIND
	{
		ACTUAL,
		RENDER,
		KIND_END
	};

private:
	explicit CMouse(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CMouse(const CMouse& _rOther);
	virtual ~CMouse() = default;

public:
	HRESULT Change_State(CMouse::STATE _eState);

	const POINT& Get_Point();

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_UI() override;

public:
	static CMouse* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;
private:
	HRESULT Add_Component();



private:
	POINT			m_tPoint = {};
	CTransform*		m_pTransformComp[KIND_END] = {};
	CTexture*		m_pTextrueComp[STATE_END] = {};

	STATE			m_eState = STATE::IDLE;
	_uint			m_iCurrFrame = 0;
};

END

#endif // !__MOUSE_H__
