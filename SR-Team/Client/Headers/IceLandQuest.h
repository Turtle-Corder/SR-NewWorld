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
	eIceQuest_ID Get_SituationID() { return m_eSituation; }
	_bool Get_Chart() { return m_bChat; }
	_bool Get_StartDeadCnt() { return m_bStartDeadCnt; }

public:
	void Set_StartQuest(_bool bStart) { m_bStartQuest = bStart; }
	void Dead_Monster();

public:
	// CUIObject��(��) ���� ��ӵ�
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

	// ������ ȹ���ߴ���
	_bool				m_bGetReward = false;

	// ����Ʈ ��������
	_bool				m_bStartQuest = false;

	// ���� �󸶳� �׿�����
	_bool			m_bStartDeadCnt = false;
	_int			m_iMonsetDeadCnt = 0;

	// NPC�� ��ȭ������
	_bool				m_bChat = false;
};

END
#endif // !__ICELANDQUEST_H__