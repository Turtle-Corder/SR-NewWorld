#pragma once

#ifndef __MAINQUESTNPC_H__
#define __MAINQUESTNPC_H__

#include "GameObject.h"
BEGIN(Client)
USING(Engine)

class CMainQuest_NPC : public CGameObject
{
public:
	explicit CMainQuest_NPC(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMainQuest_NPC(const CMainQuest_NPC& _rOther);
	virtual ~CMainQuest_NPC() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Add_Component_Transform();
	HRESULT Add_Component_Texture();

public:
	static CMainQuest_NPC* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*			m_pVIBufferCom[PART_END];
	CTransform*			m_pTransformCom[PART_END];
	CTexture*			m_pTextureCom[PART_END];
};

END
#endif // !__MAINQUESTNPC_H__
