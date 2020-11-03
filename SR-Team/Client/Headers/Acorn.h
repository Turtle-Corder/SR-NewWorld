#pragma once
#ifndef __ACORN_H__
#define __ACORN_H__

#include "GameObject.h"
USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CAcorn final : public CGameObject
{
	enum STATE { IDLE, MOVE, FALL, ATTACK };
private:
	explicit CAcorn(LPDIRECT3DDEVICE9 _pDevice);
	explicit CAcorn(const CAcorn& _rOther);
	virtual ~CAcorn() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT	Render_NoneAlpha() override;
public:
	virtual void Free() override;
	static CAcorn* Create(LPDIRECT3DDEVICE9 _pDevice);

private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT FallDown_Acorn(_float _fDeltaTime);
	HRESULT Player_Position_Confirm();
	HRESULT Move(_float _fDeltaTime);
	HRESULT	Attack(_float _fDeltaTime);
	HRESULT Update_State();
	HRESULT Spawn_AcornExplosion(const wstring& LayerTag);
private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

	_vec3				m_vPlayerPos = {}; //바닥에 닿았을때 플레이어 위치를 받아서 나아갈 예정
	_bool				m_bDead = false;
	_vec3				m_vStartPos = {};
	_float				m_fJumpPower = 2.f;
	_float				m_fJumpTime = 0.f;
	_vec3				m_vDir = {};
	_bool				m_bFallDown = true;
	STATE				m_ePreState = FALL;
	STATE				m_eCurState = FALL;
	INSTANTIMPACT		m_tInstant = {};
};

END

#endif //__ACORN_H__
