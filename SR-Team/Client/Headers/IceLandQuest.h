#pragma once

#ifndef __ICELANDQUEST_H__
#define __ICELANDQUEST_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CIceLandQuest : public CUIObject
{
public:
	enum ICEQUEST_CLEAR_STATE
	{
		ICEQUEST_NOCLEAR, ICEQUEST_CLEAR, ICEQUEST_STATE_END
	};

public:
	explicit CIceLandQuest(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CIceLandQuest(const CIceLandQuest& other);
	virtual ~CIceLandQuest() = default;

public:
	_bool Get_StartQuest() { return m_bStartQuest; }
	eIceQuest_ID Get_FlowerQuestID() { return m_eSituation; }

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

public:
	static CIceLandQuest* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture*			m_pTextureWnd[ICEQUEST_END] = { nullptr, };
	eIceQuest_ID		m_eSituation = ICEQUEST_END;

	_bool				m_bClear = false;

	// HelpWnd
	CTexture*			m_pTextureHelp[ICEQUEST_STATE_END] = { nullptr, };
	_bool				m_bRenderClear = false;

	// 보상을 획득했는지
	_bool				m_bGetReward = false;

	// 퀘스트 시작할지
	_bool				m_bStartQuest = false;
};

END
#endif // !__ICELANDQUEST_H__