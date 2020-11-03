#pragma once
#ifndef __SLIME_H__
#define __SLIME_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CSlime final : public CGameObject
{
	enum SLIME
	{
		SLIME_BODY,
		SLIME_BASE,
		SLIME_JELLY = SLIME_BASE,
		SLIME_END
	};
	enum STATE { IDLE , MOVE , ATTACK};
private:
	explicit CSlime(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSlime(const CSlime& _rOther);
	virtual ~CSlime() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_BlendAlpha() override;
	virtual HRESULT Render_NoneAlpha() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;
private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	//----------------------------------
	HRESULT IsOnTerrain();
	HRESULT	Jumping(_float _fDeltaTime);
	HRESULT LookAtPlayer(_float _fDeltaTime);
	//----------------------------------
	HRESULT Compare_PlayerPosition();
	HRESULT Divide_Cube(const wstring& LayerTag);
	HRESULT Spawn_Item(const wstring& LayerTag);
	HRESULT Spawn_Crack(const wstring& LayerTag);
	HRESULT Spawn_InstantImpact(const wstring& LayerTag);
	HRESULT Setting_SlimeBody();
	HRESULT Setting_SlimeJelly();
	HRESULT Attack(_float _fDeltaTime);
	HRESULT Update_State();
	HRESULT Move(_float _fDeltaTime);
public:
	static CSlime* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	//----------------------------------
	//----------------------------------
private:
	CVIBuffer*			m_pVIBufferCom[SLIME_END] = {};
	CTransform*			m_pTransformCom[SLIME_END] = {};
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;
	
	//	CStatus*	m_pStatCom = nullptr;

	_bool		m_bDead = false;
	_bool		m_bJump = false;
	_float		m_fJumpPower = 5.f;
	_float		m_fJumpTime = 0.f;
	_int		m_iMaxCount = 4;
	_int		m_iCurCount = 1;
	_vec3		m_vStartPos = {};
	STATE		m_ePreState;
	STATE		m_eCurState;
	_float		m_fDistance = 0.f;
	_float		m_fStartTime = 0.f;
	_bool		m_bAttackJump = false;
	SLIMEINFO   m_tSlimeInfo = {};
};

END

#endif // !__SLIME_H__

