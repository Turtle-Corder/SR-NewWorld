#pragma once
#ifndef __STUMP_PARTICLE_H__
#define __STUMP_PARTICLE_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CStump_Particle final : public CGameObject
{
private:
	explicit CStump_Particle(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStump_Particle(const CStump_Particle& _rOther);
	virtual ~CStump_Particle() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Particle_Move(_float _fDeltaTime);
	HRESULT Set_WorldMatrix(_float _fDeltaTime);
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	static CStump_Particle* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;

private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;


	INSTANTIMPACT		m_tInstant = {};
	_float				m_fDeadTime = 0.f;
	_float				m_fTimeFlow = 0.f;
	_uint				m_iTexture_Number = 0;
	_float				m_fAngle = 0.f;
	_matrix				m_matStumpWorld = {};
};

END


#endif //__STUMP_PARTICLE_H__