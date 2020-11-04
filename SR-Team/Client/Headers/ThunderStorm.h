#pragma once
#ifndef __THUNDER_STORM_H__
#define __THUNDER_STORM_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CThunderStorm final : public CGameObject
{
private:
	explicit CThunderStorm(LPDIRECT3DDEVICE9 _pDevice);
	explicit CThunderStorm(const CThunderStorm& _rOther);
	virtual ~CThunderStorm() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Render_NoneAlpha() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;
public:
	static CThunderStorm* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;
private:
	HRESULT Add_Component();
private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

	_bool				m_bDead = false;
	_bool				m_bShieldMain_RenderCheck = false;
	INSTANTIMPACT		m_tInstant = {};
	_float				m_fMoveTime = 0.f;
	_float				m_fDeadTime = 0.f;
};

END

#endif // !__THUNDER_STORM_H__

