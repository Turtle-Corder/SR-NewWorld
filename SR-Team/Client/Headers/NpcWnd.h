#pragma once

#ifndef __NPCWND_H__
#define __NPCWND_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CNpcWnd : public CUIObject
{
public:
	explicit CNpcWnd(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CNpcWnd(const CNpcWnd& other);
	virtual ~CNpcWnd();

public:
	eNormalNPC_ID Get_NpcID() { return m_eNpcID; }
	_bool Get_Chart() { return m_bChat; }

public:
	void Set_NpcID(eNormalNPC_ID eID) { m_eNpcID = eID; }

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
	static CNpcWnd* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CTexture*		m_pTextureCom[STAGE_NPC_END] = { nullptr, };
	eNormalNPC_ID	m_eNpcID = STAGE_NPC_END;

	// NPC와 대화중인지
	_bool				m_bChat = false;
};

END
#endif // !__NPCWND_H__