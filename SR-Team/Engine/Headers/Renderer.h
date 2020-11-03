#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CRenderer final : public CBase
{
public:
	enum eRENDER_TYPE
	{
		RENDER_PRIORITY,
		RENDER_NONEALPHA,
		RENDER_BLNEDALPHA,
		RENDER_ONLYALPHA,
		RENDER_UI,
		RENDER_MOUSE,
		RENDER_END,
	};

private:
	explicit CRenderer(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual ~CRenderer() = default;

public:
	HRESULT Setup_Renderer();
	HRESULT Add_RendererList(CRenderer::eRENDER_TYPE _eType, CGameObject* _pObject);
	HRESULT Render_Renderer();

private:
	HRESULT Render_Priority();
	HRESULT Render_NoneAlpha();
	HRESULT Render_BlendAlpha();
	HRESULT Render_OnlyAlpha();
	HRESULT Render_UI();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual void Free() override;



private:
	typedef list<CGameObject*> GAMEOBJECTS;
	GAMEOBJECTS m_GameObjects[RENDER_END];

	LPDIRECT3DDEVICE9	m_pDevice	= NULL;
	LPD3DXSPRITE		m_pSprite	= NULL;
	LPD3DXFONT			m_pFont		= NULL;
};

END

#endif
