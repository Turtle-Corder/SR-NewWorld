#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CBomb final : public CGameObject
{
	enum { IDLE, BOMB };
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

	virtual void Free() override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	static CBomb* Create(LPDIRECT3DDEVICE9 _pDevice);


private:
	//------------------------------------------------------------
	// Add Component
	//------------------------------------------------------------
	HRESULT Add_Component();

	HRESULT Add_Component_VIBuffer();
	HRESULT Add_Component_Transform();
	HRESULT Add_Component_Texture();
	HRESULT Add_Component_Extends();



	//------------------------------------------------------------
	// Transformation 변경
	//------------------------------------------------------------
	HRESULT IsOnTerrain(_float _fDeltaTime);



	//------------------------------------------------------------
	// 상태 변경
	//------------------------------------------------------------
public:
	HRESULT Dead_Bomb(_float _fDeltaTime);
	//------------------------------------------------------------
	// Variable (변수)
	//------------------------------------------------------------
private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

	_float				m_fJumpPower = 2.f;
	_float				m_fJumpTime = 0.f;
	_float				m_fBombTime = 0.f;

	_bool				m_bDead = false;
	_uint				m_iTexCnt = 0;
	_int				m_iAnimationStep = 0;



	INSTANTIMPACT		m_tInstant = {};
};

END

#endif //__BOMB_H__