#pragma once
#ifndef __STUMP_H__
#define __STUMP_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

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
	enum STATE { IDLE, MOVE, ATTACK };
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
	HRESULT Compare_PlayerPosition();
	HRESULT IsOnTerrain();
	HRESULT	Move(_float _fDeltaTime);
	HRESULT LookAtPlayer(_float _fDeltaTime);
	HRESULT Attack(_float _fDeltaTime);
	HRESULT Setting_Part();
	HRESULT Spawn_Acorn(const wstring& LayerTag, _uint _iCount);
private:
	CVIBuffer*			m_pVIBufferCom[STUMP_END] = {};
	CTransform*			m_pTransformCom[STUMP_END] = {};
	CTexture*			m_pTextureCom[STUMP_END] = {};
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

private:
	_vec3				m_vStartPos = {};
	_vec3				m_vDir = {};
	_vec3				m_vLook = {};
	_vec3				m_vPrePos = {};
	_bool				m_bAcorn_CreateOne_Check = false;
	STATE				m_ePreState = IDLE;
	STATE				m_eCurState = IDLE;
};

END

#endif //__STUMP_H__