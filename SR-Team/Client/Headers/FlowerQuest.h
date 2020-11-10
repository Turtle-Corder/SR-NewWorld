#pragma once

#ifndef __FLOWERQUEST_H__
#define __FLOWERQUEST_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CFlowerQuest : public CUIObject
{
public:
	enum CLEAR_STATE
	{
		NO_CLEAR, CLEAR, CLEAR_STATE_END
	};

public:
	explicit CFlowerQuest(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CFlowerQuest(const CFlowerQuest& other);
	virtual ~CFlowerQuest() = default;

public:
	_bool Get_StartQuest() { return m_bStartQuest; }
	eFlowerQuest_ID Get_FlowerQuestID() { return m_eSituation; }
	_bool Get_Chart() { return m_bChat; }

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
	static CFlowerQuest* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture*			m_pTextureWnd[FLOWER_QUEST_END] = { nullptr, };
	eFlowerQuest_ID		m_eSituation = FLOWER_QUEST_END;

	RECT				m_tQuestWndCollRt = { 0, };
	_bool				m_bClear = false;

	// HelpWnd
	CTexture*			m_pTextureHelp[CLEAR_STATE_END] = { nullptr, };
	_bool				m_bRenderClear = false;

	// 보상을 획득했는지
	_bool				m_bGetReward = false;

	// 퀘스트 시작할지
	_bool				m_bStartQuest = false;

	// NPC와 대화중인지
	_bool				m_bChat = false;
};

END
#endif