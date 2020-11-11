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
	enum STATE { IDLE, MOVE, ATTACK, ATTACK1 };

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
	virtual HRESULT Render_BlendAlpha() override;

	virtual void Free() override;
	static  CStump* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Set_Active() override;

	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;

private:
	HRESULT Add_Component();

	HRESULT Update_AI();
	HRESULT IsOnTerrain();
	HRESULT Update_Move(_float _fDeltaTime);

	HRESULT Update_Transform();
	HRESULT Update_State(_float _fDeltaTime);






	HRESULT Update_Animation(_float _fDeltaTime);
	HRESULT Update_Animation_Move(_float _fDeltaTime);
	HRESULT Update_Animation_Attack(_float _fDeltaTime);
	HRESULT Update_Animation_Attack2(_float _fDeltaTime);
	HRESULT Anim_Reset_Move();
	HRESULT Anim_Reset_Attack();
	HRESULT Spawn_Acorn(const wstring& LayerTag, _uint _iCount);
	HRESULT Spawn_StumpImpact(const wstring& LayerTag);
	HRESULT Make_Rubble();
	void Update_AttackDelay(_float _fDeltaTime);
	void Update_HurtDelay(_float _fDeltaTime);
	void Update_FlinchDelay(_float _fDeltaTime);

private:
	CVIBuffer*			m_pVIBufferCom[STUMP_END] = {};
	CTransform*			m_pTransformCom[STUMP_END] = {};
	CTexture*			m_pTextureCom[STUMP_END] = {};
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

private:
	//--------------------------------------------------
	// ����
	//--------------------------------------------------
	STATE		m_ePreState = IDLE;
	STATE		m_eCurState = IDLE;

	_vec3		m_vSpawnPos = {};
	_vec3		m_vMoveDirection = {};		// �̵��� or �Ĵ� �� ����
	_float		m_fFollowDistance = 10.f;	// ���� ������ �Ÿ�
	_float		m_fFollowLimitNear = 4.f;
	_float		m_fAttackDistance = 4.f;	// ���� ������ �Ÿ�


	//--------------------------------------------------
	// ����
	//--------------------------------------------------
	_bool		m_bCanAttack = true;		// ���� ���� or �Ұ���
	_float		m_fAttackDelay = 10.f;		// ���� ������ ������
	_float		m_fAttackTimer = 0.f;		// ���� ��Ÿ�� �ð� ��¿�

	INSTANTIMPACT	m_tImpact = {};
	_bool		m_bSpawnImpact = false;

	_float		m_fDistance = 0.f;

	//--------------------------------------------------
	// �ǰ�
	//--------------------------------------------------
	_bool		m_bCanHurt = true;			// �ǰ� ���� or �Ұ���
	_float		m_fHurtDelay = 0.1f;		// �ǰ� ������ ������
	_float		m_fHurtTimer = 0.f;			// �ǰ� ��Ÿ�� �ð� ��¿�

	CTexture*	m_pFlinchTexCom = nullptr;
	_bool		m_bFlinch = false;
	_float		m_fFlinchTimer = 0.f;
	_float		m_fFlinchDealy = 0.1f;

	//--------------------------------------------------
	// �ִϸ��̼�
	//--------------------------------------------------
	_int		m_iAnimationStep = 0;
	_float		m_fAnimationTimer = 0.f;
	_float		m_fAnimationSpeed = 1.9f;
};

END

#endif //__STUMP_H__