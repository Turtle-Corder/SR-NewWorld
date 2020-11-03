#pragma once
#ifndef __PINK_BEAN_H__
#define __PINK_BEAN_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CPinkBean final : public CGameObject
{
private:
	explicit CPinkBean(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPinkBean(const CPinkBean& _rOther);
	virtual ~CPinkBean() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static  CPinkBean* Create(LPDIRECT3DDEVICE9 _pDevice);
private:
	//HRESULT	Late_Setup();
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
private:
	CVIBuffer*		m_pVIBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;

	//----------------------------------------------
	//----------------------------------------------
	_float			m_fAttackTime = 0.f;
	_int			m_iRandAttack = -1;
	_uint			m_iCreateCnt = 0;
	_vec3			m_vStartPos = {};
	_vec3			m_vTemp = {};
	_vec3 			m_vRand[5] = {};
	_bool			m_bDead = false;
	_bool			m_bAttack = false;
	_bool			m_bAttackStart = false;
	_bool			m_bAttackDelay = false;
	_bool			m_bCreate_Bomb = false;
	_bool			m_bMonSubOn = false;
	//list<CGameObject*> m_vecSub = {}; //핑크빈에서 쓸거
	//_bool			m_bLateSetUp = false;
	// 기준이 될녀석	=> Transform만
	// 나머지는 싹다
};


END
#endif //__PINK_BEAN_H__
