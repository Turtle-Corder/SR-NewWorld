#pragma once

#ifndef __QUESTITEMFLOWER_H__
#define __QUESTITEMFLOWER_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CQuestItem_Flower : public CUIObject
{
public:
	enum LOADING
	{
		LOADING_BACKGROUND, LOADING_BAR, LOADING_END
	};

public:
	explicit CQuestItem_Flower(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CQuestItem_Flower(const CQuestItem_Flower& other);
	virtual ~CQuestItem_Flower() = default;

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
	static CQuestItem_Flower* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture*		m_pTextureLoading[LOADING_END] = { nullptr, };
	CTransform*		m_pTransformLoading[LOADING_END] = { nullptr, };
	CTexture*		m_pTextureFlower = nullptr;

	_bool			m_bRenderLoadingBar = false;
};

END
#endif