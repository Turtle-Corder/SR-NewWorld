#pragma once
#ifndef __YETI_H__
#define __YETI_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDamageInfo;
class CYeti final : public CGameObject
{
	enum YETI 
	{ 
		YETI_BASE,
		YETI_ROOT = YETI_BASE,
		YETI_CENTER,
		YETI_BODY,
		YETI_HEAD,
		YETI_LEFT,
		YETI_RIGHT,
		YETI_LEFTLEG,
		YETI_RIGHTLEG,
		YETI_END 
	};
	enum STATE {IDLE , MOVE , ATTACK , DEAD};
	enum CHANGE {CHANGE_LEFT , CHANGE_RIGHT , CHANGE_END };
private:
	explicit CYeti(LPDIRECT3DDEVICE9 pDevice);
	explicit CYeti(const CYeti& other);
	virtual ~CYeti() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
	virtual HRESULT Render_BlendAlpha() override;

	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;
private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Setting_Part(float _fDeltaTime);
	HRESULT Moving(float _fDeltaTime);
	HRESULT Attack(float _fDeltaTime);
	HRESULT Update_State();
	HRESULT MoveMotion(_float _fDeltaTime);

	void Update_FlinchDelay(_float _fDeltaTime);
	void Update_HurtDelay(_float _fDeltaTime);

public:
	static CYeti* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free() override;
	HRESULT LookAtPlayer(float _fDeltaTime);
	HRESULT Spawn_Snow(const wstring& LayerTag);

	virtual void Set_Active() override;



private:
	CVIBuffer*			m_pVIBufferCom[YETI_END];
	CTransform*			m_pTransformCom[YETI_END];
	CTexture*			m_pTextureCom[YETI_END];
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;
	

	_bool		m_bCanHurt = true;			// 피격 가능 or 불가능
	_float		m_fHurtDelay = 0.1f;		// 피격 가능한 딜레이
	_float		m_fHurtTimer = 0.f;

	_bool		m_bJump = false;
	_float		m_fJumpPower = 5.f;
	_float		m_fJumpTime = 0.f;
	_float		m_fAngle = 0.f;
	_bool		m_bAttack = false;
	_bool		m_bAttackDelay = false;
	_bool		m_bRHandDown = false;
	_bool		m_bHighestCheck = false;
	_float		m_fAttackTime = 0.f;
	_vec3		m_vStartPos = {};
	_float		m_fWalkTime = 0.f;
	_float		m_fTime = 0.f;
	INSTANTIMPACT*	m_pInstantImpact = nullptr;
	STATE		m_ePreState;
	STATE		m_eCurState;
	_float		m_fMoveTime = 0.f;
	CHANGE		m_eMove = CHANGE_LEFT;

	CTexture*		m_pFlinchTexCom = nullptr;

	_bool			m_bFlinch = false;
	_float			m_fFlinchTimer = 0.f;
	_float			m_fFlinchDealy = 0.1f;
};

END

#endif // !__YETI_H__
