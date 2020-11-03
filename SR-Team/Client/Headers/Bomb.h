#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CBomb final : public CGameObject
{
private:
	explicit CBomb(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBomb(const CBomb& _rOther);
	virtual ~CBomb() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT IsOnTerrain(_float _fDeltaTime);
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static CBomb* Create(LPDIRECT3DDEVICE9 _pDevice);
	HRESULT Setting_Dir();
	HRESULT Dead_Bomb(_float _fDeltaTime);
private:
	CVIBuffer*		m_pVIBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;


	_vec3			m_vStartPos = {};
	_float			m_fJumpPower = 2.f;
	_float			m_fJumpTime = 0.f;
	_bool			m_bFallDown = false;
	_bool			m_bOnece = false;
	_vec3			m_vDir = {};
	_bool			m_bDead = false;
	_bool			m_bStart = false;
	_uint			m_iTexCnt = 0;
	_float			m_fBombTime = 0.f;
	_float			m_fDeadTime = 0.f;
};

END

#endif //__BOMB_H__