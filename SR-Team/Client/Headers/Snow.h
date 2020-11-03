#pragma once
#ifndef __SNOW_H__
#define __SNOW_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDamageInfo;

class CSnow final : public CGameObject
{
private:
	explicit CSnow(LPDIRECT3DDEVICE9 pDevice);
	explicit CSnow(const CSnow& other);
	virtual ~CSnow() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT Setting_Dir();
public:
	static CSnow* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;
public:
	HRESULT Throwing_Snow(float _fDeltaTime);



private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusComp = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;
	bool				m_bDead = false;
	bool				m_bJump = false;
	float				m_fJumpPower = 1.5f;
	float				m_fJumpTime = 0.f;
	_vec3				m_vPos = {};
	_vec3				m_vDir = {};
	bool				m_bCheck = false;
	bool				m_bFallDown = false;
	bool				m_bGround = false;
	float				m_fGravity = 0.f;
	_float				m_fDeadTime = 0.f;

	INSTANTIMPACT		m_tInstant = {};
};

END

#endif // !__SNOW_H__
