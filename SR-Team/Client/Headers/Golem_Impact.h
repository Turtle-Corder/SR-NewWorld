#pragma once
#ifndef __GOLEM_IMPACT_H__
#define __GOLEM_IMPACT_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CGolem_Impact final : public CGameObject
{
private:
	explicit CGolem_Impact(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGolem_Impact(const CGolem_Impact& _rOther);
	virtual ~CGolem_Impact() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
private:
	HRESULT Add_Component();
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static CGolem_Impact* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;



private:
	CTransform*			m_pTransformCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

	_vec3			m_vPos = {};
	_bool			m_bDead = false;
	INSTANTIMPACT	m_tInstant = {};

	_float			m_fDeadTime = 0.f;
};

END

#endif //__INSTANT_IMPACT_H__