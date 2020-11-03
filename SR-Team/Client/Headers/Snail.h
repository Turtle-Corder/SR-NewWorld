#pragma once
#ifndef __SNAIL_H__
#define __SNAIL_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDamageInfo;

class CSnail final : public CGameObject
{
	enum SNAIL { SNAIL_HEAD, SNAIL_BODY, SNAIL_END };
	enum STATE {IDLE , MOVE , ATTACK};
private:
	explicit CSnail(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSnail(const CSnail& _rOther);
	virtual ~CSnail() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(_float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Update_State();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT	Move(_float _fDeltaTime);
	HRESULT LookAtPlayer(_float _fDeltaTime);
	HRESULT Attack(_float _fDeltaTime);
	HRESULT Setting_Part();
	HRESULT Spawn_InstantImpact(const wstring& LayerTag);

public:
	static CSnail* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;

private:
	CVIBuffer*			m_pVIBufferCom[SNAIL_END] = {};
	CTransform*			m_pTransformCom[SNAIL_END] = {};
	CTexture*			m_pTextureCom[SNAIL_END] = {};
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

private:
	_vec3		m_vStartPos = {};
	_bool		m_bAttack = false;
	_vec3		m_vDir = {};
	INSTANTIMPACT*	m_pInstantImpact = nullptr;
	STATE		m_ePreState = IDLE;
	STATE		m_eCurState = IDLE;
	_bool		m_bHit = false;
	_vec3		m_vLook = {};
	_bool		m_bCheck = false;
	_bool		m_bCrash = false;
	_vec3		m_vPrePos = {};

	_bool		m_bInstanceCreate = false;
	_bool		m_bTakeCheckOnece = false;
};

END

#endif // !__SNAIL_H__
