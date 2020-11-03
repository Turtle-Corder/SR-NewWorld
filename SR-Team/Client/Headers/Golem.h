#pragma once
#ifndef __GOLEM_H__
#define __GOLEM_H__
#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CGolem final : public CGameObject
{
	enum GOLEM
	{
		GOLEM_BASE,
		GOLEM_ROOT = GOLEM_BASE,
		GOLEM_CENTER,
		GOLEM_BODY,
		GOLEM_HEAD,
		GOLEM_LEFT_ARM,
		GOLEM_RIGHT_ARM,
		GOLEM_LEFT_LEG,
		GOLEM_RIGHT_LEG,
		GOLEM_END
	};
	enum STATE {IDLE , MOVE , ATTACK , ATTACK2 , ATTACK3 , STATE_DEAD};
private:
	explicit CGolem(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGolem(const CGolem& _rOther);
	virtual ~CGolem() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static  CGolem* Create(LPDIRECT3DDEVICE9 _pDevice);
private:
	HRESULT Attack(_float _fDeltaTime);
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Setting_Part();
	HRESULT Setting_Center();
	HRESULT Create_Bomb(_float _fDeltaTime);
	HRESULT Create_MonSub(_float _fDeltaTime);
	HRESULT Spawn_LayerBomb(const wstring & LayerTag, _vec3 _vPos);
	HRESULT Spawn_Layer_MonSub(const wstring & LayerTag, _vec3 _vPos);
	HRESULT	Move(_float _fDeltaTIme);
	HRESULT LookAtPlayer(_float _fDeltaTime);
	HRESULT Update_State();
private:
	CVIBuffer*		m_pVIBufferCom[GOLEM_END] = {};
	CTransform*		m_pTransformCom[GOLEM_END] = {};
	CTexture*		m_pTextureCom[GOLEM_END] = {};

	//----------------------------------------------
	//----------------------------------------------
	_int			m_iRandAttack = -1;
	_uint			m_iCreateCnt = 0;
	_vec3			m_vStartPos = {};
	_vec3			m_vTemp = {};
	_vec3 			m_vBombRand[5] = {};
	_vec3 			m_vMonSubRand[4] = {};
	_bool			m_bDead = false;
	_bool			m_bAttack = false;
	_bool			m_bAttackDelay = false;
	_bool			m_bCreate_Bomb = false;
	_bool			m_bMonSubOn = false;
	_bool			m_bDirCheck = false;
	_bool			m_bMove = false;
	_bool			m_bLook = false;
	_float			m_fRad = 0.f;
	_float			m_fLimit = 0.f;
	_float			m_fLength = 0.f;
	_vec3			m_vDir = {};
	_vec3			m_vPos = {};
	_int			m_iAttackPattern = 0;
	_float			m_fTime = 0.f;
	_float			m_fAttTime = 0.f;
	_float			m_fHandAttackTime = 0.f;
	_uint			m_iMonSubCreateCnt = 0;
};

END
#endif //__GOLEM_H__