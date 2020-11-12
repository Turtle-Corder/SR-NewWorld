#pragma once
#ifndef __SNAIL_IMPACT_H__
#define __SNAIL_IMPACT_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CSnail_Impact final : public CGameObject
{
private:
	explicit CSnail_Impact(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSnail_Impact(const CSnail_Impact& _rOther);
	virtual ~CSnail_Impact() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
private:
	HRESULT Add_Component();
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);
	virtual void Free() override;
	static CSnail_Impact* Create(LPDIRECT3DDEVICE9 _pDevice);
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

#endif //__SNAIL_IMPACT_H__
