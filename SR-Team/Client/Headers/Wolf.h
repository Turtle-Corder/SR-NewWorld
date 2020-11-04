#pragma once
#ifndef __WOLF_H__
#define __WOLF_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CWolf final : public CGameObject
{
	enum WOLF
	{
		WOLF_BASE,
		WOLF_BODY,
		WOLF_NECK,
		WOLF_HEAD,
		WOLF_MOUTH,
		WOLF_EAR1,
		WOLF_EAR2,
		WOLF_LEG1,
		WOLF_LEG2,
		WOLF_LEG3,
		WOLF_LEG4,
		WOLF_END
	};
	enum STATE { IDLE, MOVE, ATTACK, STATE_DEAD };
	enum CHANGE { CHANGE_LEFT, CHANGE_RIGHT, CHANGE_END };
	enum SHAKE { SHAKE_LHEAD, SHAKE_RHEAD, SHAKE_END };
private:
	explicit CWolf(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWolf(const CWolf& _rOther);
	virtual ~CWolf() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Render_NoneAlpha() override;
public:
	virtual void Free() override;
	static  CWolf* Create(LPDIRECT3DDEVICE9 _pDevice);
private:
	HRESULT Add_Component();
	HRESULT Update_State();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT	Move(_float _fDeltaTime);
	HRESULT LookAtPlayer(_float _fDeltaTime);
	HRESULT Attack(_float _fDeltaTime);
	HRESULT Setting_Part();
	HRESULT MoveMotion(_float _fDeltaTime);
	HRESULT AttackMotion(_float _fDeltaTime);
	HRESULT Spawn_InstantImpact(const wstring& LayerTag);
private:
	CVIBuffer*			m_pVIBufferCom[WOLF_END] = {};
	CTransform*			m_pTransformCom[WOLF_END] = {};
	CTexture*			m_pTextureCom[WOLF_END] = {};
	CSphereCollider*	m_pColliderCom = nullptr;


private:
	_vec3		m_vStartPos = {};
	_bool		m_bAttack = false;
	_vec3		m_vDir = {};
	INSTANTIMPACT*	m_pInstantImpact = nullptr;
	STATE		m_ePreState;
	STATE		m_eCurState;
	_bool		m_bHit = false;
	_vec3		m_vLook = {};
	_bool		m_bCheck = false;
	_bool		m_bCrash = false;
	_vec3		m_vPrePos = {};

	_float		m_fMoveTime = 0.f;
	CHANGE		m_eMove = CHANGE_LEFT;
	_float		m_fHeadShakeTime = 0.f;
	SHAKE		m_eHead = SHAKE_LHEAD;
};

END

#endif // !__WOLF_H__
