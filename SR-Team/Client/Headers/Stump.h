#pragma once
#ifndef __STUMP_H__
#define __STUMP_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStump final : public CGameObject
{
	enum STUMP
	{
		STUMP_BASE,
		STUMP_BODY,
		STUMP_LSHD,
		STUMP_RSHD,
		STUMP_LH,
		STUMP_RH,
		STUMP_LEG1,
		STUMP_LEG2,
		STUMP_LEG3,
		STUMP_LEG4,
		STUMP_END
	};
	enum STATE { IDLE, MOVE, ATTACK, STATE_DEAD };
private:
	explicit CStump(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStump(const CStump& _rOther);
	virtual ~CStump() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Render_NoneAlpha() override;
public:
	virtual void Free() override;
	static  CStump* Create(LPDIRECT3DDEVICE9 _pDevice);
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
private:
	CVIBuffer*		m_pVIBufferCom[STUMP_END] = {};
	CTransform*		m_pTransformCom[STUMP_END] = {};
	CTexture*		m_pTextureCom[STUMP_END] = {};


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
};

END

#endif //__STUMP_H__