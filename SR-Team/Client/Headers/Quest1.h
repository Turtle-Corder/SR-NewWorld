#pragma once

#ifndef __QUEST1_H__
#define __QUEST1_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CQuest1 : public CUIObject
{
public:
	explicit CQuest1(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CQuest1(const CQuest1& other);
	virtual ~CQuest1() = default;

public:
	void Change_QuestState(eQuest1_ID eQuestID) { m_eSituation = eQuestID; }

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

public:
	static CQuest1* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture*		m_pTextureCom[QUEST1_END] = { nullptr, };

	eQuest1_ID		m_eSituation = QUEST1_END;
	RECT			m_tGreetingCollRt = { 0, };
	RECT			m_tQuestWndCollRt = { 0, };
	_bool			m_bRenderClear = false;
};


END
#endif // !__QUEST1_H__
