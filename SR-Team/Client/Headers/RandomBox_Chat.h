#pragma once

#ifndef __RANDOMBOXCHAT_H__
#define __RANDOMBOXCHAT_H_

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CRandomBox_Chat : public CUIObject
{
public:
	enum RANDOMBOX_SORT
	{
		POTION_BOX, EQUIP_BOX, END_BOX
	};

public:
	explicit CRandomBox_Chat(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CRandomBox_Chat(const CRandomBox_Chat& other);
	virtual ~CRandomBox_Chat() = default;

public:
	_bool Get_Chat() { return m_bChat; }

public:
	void Set_StartChat(_bool bStart) { m_bStartChat = bStart; }

public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_UI() override;

private:
	HRESULT Render_RandomBox();
	HRESULT Render_RandomBoxInfo();
	HRESULT Render_ClearWnd();

private:
	HRESULT Add_Component();

public:
	static CRandomBox_Chat* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	HRESULT Getcha_PotionBox();
	HRESULT Getcha_EquipBox();

private:
	CTexture*					m_pTextureWnd[RANDOMBOX_END] = { nullptr, };
	CTexture*					m_pTextureBox[END_BOX] = { nullptr, };
	CTexture*					m_pTextureInfo[END_BOX] = { nullptr, };
	CTexture*					m_pTextureClear = nullptr;
	eRandomBoxChatWnd_ID		m_eSituation = RANDOMBOX_END;

	_bool						m_bStartChat = false;
	_bool						m_bEnd = false;
	_bool						m_bChat = false;
	_bool						m_bIsColl = false;

	_bool						m_bRenderPotionInfo = false;
	_bool						m_bRenderEquipInfo = false;
	_bool						m_bRenderClearWnd = false;

	RANDOMBOX_SORT				m_eSelectedBox = END_BOX;
	RECT						m_tRBoxCollRt[END_BOX] = { 0, };
	_vec3						m_vBoxPos[END_BOX];
};


END
#endif // !__RANDOMBOXCHAT_H__
