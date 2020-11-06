#pragma once
#ifndef __ENERGY_BOLT_H__
#define __ENERGY_BOLT_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CEnergyBolt final : public CGameObject
{
private:
	explicit CEnergyBolt(LPDIRECT3DDEVICE9 _pDevice);
	explicit CEnergyBolt(const CEnergyBolt& _rOther);
	virtual ~CEnergyBolt() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha();
private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT Move(_float _fDeltaTime);
public:
	virtual CGameObject* Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static CEnergyBolt* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);
private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusComp = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

	INSTANTIMPACT		m_tInstant = {};
	_bool				m_bDead = false;
	_bool				m_bOnece = false;
	_float				m_fTestDeadTime = 0.f;
	_vec3				m_vPlayerLook = {};
};

END

#endif //__ENERGY_BOLT_H__