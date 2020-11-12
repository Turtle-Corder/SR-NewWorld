#pragma once
#ifndef __ACORN_EXLPOSION_H__
#define __ACORN_EXLPOSION_H__

#include "GameObject.h"
USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CAcornExplosion final : public CGameObject
{
private:
	explicit CAcornExplosion(LPDIRECT3DDEVICE9 _pDevice);
	explicit CAcornExplosion(const CAcornExplosion& _rOther);
	virtual ~CAcornExplosion() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_OnlyAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
public:
	static CAcornExplosion* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;
private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

	_bool				m_bDead = false;
	_vec3				m_vStartPos = {};
	_uint				m_iCurrFrame = 0;
	_uint				m_iMaxCnt = 0;
	_float				m_fDeadTime = 0.f;
	_vec3				m_vDir = {};
	INSTANTIMPACT		m_tInstant = {};
};

END

#endif // !__ACORN_EXLPOSION_H__
