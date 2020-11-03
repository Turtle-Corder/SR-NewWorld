#pragma once
#ifndef __SLIME_IMPACT_H__
#define __SLIME_IMPACT_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CSlime_Impact final : public CGameObject
{
private:
	explicit CSlime_Impact(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSlime_Impact(const CSlime_Impact& _rOther);
	virtual ~CSlime_Impact() = default;
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
	static CSlime_Impact* Create(LPDIRECT3DDEVICE9 _pDevice);
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

#endif // !__SLIME_IMPACT_H__
