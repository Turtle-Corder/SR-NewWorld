#pragma once
#ifndef __WOLF_H__
#define __WOLF_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CWolf final : public CGameObject
{
	enum WOLF_PART
	{
		WOLF_BASE,

		WOLF_UPDATEA_START,
		WOLF_BODY = WOLF_UPDATEA_START,
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

	enum STATE { IDLE, MOVE, ATTACK, DEAD };


private:
	explicit CWolf(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWolf(const CWolf& _rOther);
	virtual ~CWolf() = default;



	//----------------------------------------------------------------------------------------------------
	// Function
	//----------------------------------------------------------------------------------------------------
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

	virtual HRESULT Render_NoneAlpha() override;

	virtual void Free() override;
	static  CWolf* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);



private:
	//--------------------------------------------------
	// Add Component
	//--------------------------------------------------
	HRESULT Add_Component();

	HRESULT Add_Component_VIBuffer();
	HRESULT Add_Component_Transform();
	HRESULT Add_Component_Texture();
	HRESULT Add_Component_Extends();


	//--------------------------------------------------
	// Transformation 변경
	//--------------------------------------------------
	HRESULT Update_AI();				// 순찰 체크
	HRESULT IsOnTerrain();
	HRESULT Update_Move(_float _fDeltaTime);
	HRESULT Update_Transform(_float _fDeltaTime);


	//--------------------------------------------------
	// 상태 변경
	//--------------------------------------------------
	HRESULT Update_State(_float _fDeltaTime);

	HRESULT Update_AtkDelay(_float _fDeltaTime);
	HRESULT Update_HurtDelay(_float _fDeltaTime);


	// 애니메이션
	HRESULT Update_Anim(_float _fDeltaTime);
	HRESULT Anim_Reset_Move();
	HRESULT Anim_Reset_Attack();
	HRESULT Update_Anim_Move(_float _fDeltaTime);
	HRESULT Update_Anim_Attack1(_float _fDeltaTime);
	HRESULT Update_Anim_Attack2(_float _fDeltaTime);

	// 공격체
	HRESULT Spawn_Impact();





	//----------------------------------------------------------------------------------------------------
	// Variable (변수)
	//----------------------------------------------------------------------------------------------------
private:
	CVIBuffer*			m_pVIBufferCom[WOLF_END] = {};
	CTransform*			m_pTransformCom[WOLF_END] = {};
	CTexture*			m_pTextureCom[WOLF_END] = {};
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;


	//--------------------------------------------------
	// 상태
	//--------------------------------------------------
	STATE		m_ePreState = IDLE;
	STATE		m_eCurState = IDLE;

	_vec3		m_vMoveDirection = {};		// 이동할 or 쳐다 볼 방향
	_float		m_fFollowDistance = 10.f;	// 추적 가능한 거리
	_float		m_fFollowLimitNear = 0.2f;
	_float		m_fAttackDistance = 3.f;	// 공격 가능한 거리


											//--------------------------------------------------
											// 공격
											//--------------------------------------------------
	_bool		m_bCanAttack = true;		// 공격 가능 or 불가능
	_float		m_fAttackDelay = 5.f;		// 공격 가능한 딜레이
	_float		m_fAttackTimer = 0.f;		// 공격 쿨타임 시간 재는용

	INSTANTIMPACT	m_tImpact = {};


	//--------------------------------------------------
	// 피격
	//--------------------------------------------------
	_bool		m_bCanHurt = true;			// 피격 가능 or 불가능
	_float		m_fHurtDelay = 5.f;			// 피격 가능한 딜레이
	_float		m_fHurtTimer = 0.f;			// 피격 쿨타임 시간 재는용


											//--------------------------------------------------
											// 애니메이션
											//--------------------------------------------------
	_int		m_iAnimStep = 0;
	_float		m_fAnimTimer = 0.f;
	_bool		m_bDead = false;
};

END

#endif // !__WOLF_H__

