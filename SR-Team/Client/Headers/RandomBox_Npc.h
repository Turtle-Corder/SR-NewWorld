#pragma once

#ifndef __RANDOMBOXNPC_H__
#define __RANDOMBOXNPC_H__

#include "GameObject.h"
BEGIN(Client)
USING(Engine)

class CRandomBox_Npc : public CGameObject
{
public:
	explicit CRandomBox_Npc(LPDIRECT3DDEVICE9 _pDevice);
	explicit CRandomBox_Npc(const CRandomBox_Npc& _rOther);
	virtual ~CRandomBox_Npc() = default;

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
	static CRandomBox_Npc* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*			m_pVIBufferCom[PART_END] = { nullptr, };
	CTransform*			m_pTransformCom[PART_END] = { nullptr, };
	CTexture*			m_pTextureCom[PART_END] = { nullptr, };
};

END
#endif