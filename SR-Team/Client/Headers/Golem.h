#pragma once
#ifndef __GOLEM_H__
#define __GOLEM_H__
#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;
class CMiniGolem;
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
	enum STATE
	{
		IDLE, MOVE,
		ATTACK1, ATTACK2, ATTACK3, ATTACK4, ATTACK6,//ATTACK5, 
		STATE_DEAD
	};
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
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;
private:
	HRESULT Add_Component();
	HRESULT Add_Component_VIBuffer();
	HRESULT Add_Component_Transform();
	HRESULT Add_Component_Texture();
	HRESULT Add_Component_Extends();

	HRESULT Update_AI();

	HRESULT IsOnTerrain();
	HRESULT Update_Move(_float _fDeltaTime);
	HRESULT Update_Transform(_float _fDeltaTime);

	HRESULT Update_State();

	HRESULT Update_Anim(_float _fDeltaTime);
	HRESULT Anim_Reset_Move();
	HRESULT Anim_Reset_Attack();

	HRESULT Update_Anim_Move(_float _fDeltaTime);
	HRESULT Update_Anim_Attack_Hand(_float _fDeltaTime);
	HRESULT Update_Anim_Attack1(_float _fDeltaTime);		// 내려찍기
	HRESULT Update_Anim_Attack2(_float _fDeltaTime);		// 폭탄
	HRESULT Update_Anim_Attack3(_float _fDeltaTime);		// 몬스터 생성
	HRESULT Update_Anim_Attack4(_float _fDeltaTime);		// 분신 소환
	HRESULT Update_Anim_Attack5(_float _fDeltaTime);		// 불 소환
	HRESULT Update_Anim_Attack6(_float _fDeltaTime);

	void Update_AttackDelay(_float _fDeltaTime);
	void Update_HurtDelay(_float _fDeltaTime);

	HRESULT Spawn_GolemImpact();
	HRESULT Spawn_Bomb();
	HRESULT Spawn_MonSub();

	HRESULT Make_Pieces();

	HRESULT Create_MiniGolem();
	//	HRESULT Spawn_Fire();


	HRESULT Phatton();
private:
	CVIBuffer*			m_pVIBufferCom[GOLEM_END] = {};
	CTransform*			m_pTransformCom[GOLEM_END] = {};
	CTexture*			m_pTextureCom[GOLEM_END] = {};
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;



	//------------------------------------------------------------
	// 상태
	//------------------------------------------------------------
	STATE			m_ePreState = IDLE;
	STATE			m_eCurState = IDLE;


	_vec3			m_vMoveDirection = {};
	_float			m_fFollowDistance = 10.f;
	_float			m_fAttackDistance = 3.f;



	//------------------------------------------------------------
	// 공격
	//------------------------------------------------------------
	_bool			m_bCanAttack = true;
	_float			m_fAttackDelay = {};
	_float			m_fAttackTimer = {};
	_int			m_iRandAttack = -1;
	_vec3			m_vDirection = {};

	//------------------------------------------------------------
	// 공격체 
	//------------------------------------------------------------
	INSTANTIMPACT	m_tImpact = {};
	_bool			m_bImpactCreateOneLimit = false;

	//------------------------------------------------------------
	// 피격 
	//------------------------------------------------------------
	_bool			m_bCanHurt = true;
	_float			m_fHurtDelay = 0.f;
	_float			m_fHurtTimer = 0.f;



	//------------------------------------------------------------
	// 애니메이션 
	//------------------------------------------------------------
	_int			m_iAnimationStep = 0;
	_float			m_fAnimationTimer = 0.f;
	_float			m_fAnimationPlayTime = 0.f;
	_float			m_fAnimationLHPositionY = 0.f;
	_float			m_fAnimationRHPositionY = 0.f;
	_vec3			m_fAnimationLHPosition = {};
	_vec3			m_fAnimationRHPosition = {};
	_bool			m_bAnimationOnce = false;
	_bool			m_bLateSetup = false;
	_bool			m_bDead = false;

	CMiniGolem*		m_pMiniGolem[5] = { nullptr, };

	_uint			m_iPattonCnt = 0;
	_float			m_fMoveSpeed = 2.f;
	_bool			m_bMiniGolem_SetPosition = false;
	_bool			m_OneCheck = false;
};

END
#endif //__GOLEM_H__