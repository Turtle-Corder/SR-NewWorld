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
	virtual HRESULT Render_BlendAlpha() override;

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
	// Transformation ����
	//--------------------------------------------------
	HRESULT Update_AI();				// ���� üũ
	HRESULT IsOnTerrain();
	HRESULT Update_Move(_float _fDeltaTime);
	HRESULT Update_Transform(_float _fDeltaTime);


	//--------------------------------------------------
	// ���� ����
	//--------------------------------------------------
	HRESULT Update_State(_float _fDeltaTime);

	HRESULT Update_AtkDelay(_float _fDeltaTime);
	HRESULT Update_HurtDelay(_float _fDeltaTime);
	HRESULT Update_FlinchDelay(_float _fDeltaTime);


	// �ִϸ��̼�
	HRESULT Update_Anim(_float _fDeltaTime);
	HRESULT Anim_Reset_Move();
	HRESULT Anim_Reset_Attack();
	HRESULT Update_Anim_Move(_float _fDeltaTime);
	HRESULT Update_Anim_Attack1(_float _fDeltaTime);
	HRESULT Update_Anim_Attack2(_float _fDeltaTime);

	// ����ü
	HRESULT Spawn_Impact();
	HRESULT Make_Blood();

	virtual void Set_Active() override;


	//----------------------------------------------------------------------------------------------------
	// Variable (����)
	//----------------------------------------------------------------------------------------------------
private:
	CVIBuffer*			m_pVIBufferCom[WOLF_END] = {};
	CTransform*			m_pTransformCom[WOLF_END] = {};
	CTexture*			m_pTextureCom[WOLF_END] = {};
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;


	//--------------------------------------------------
	// ����
	//--------------------------------------------------
	STATE		m_ePreState = IDLE;
	STATE		m_eCurState = IDLE;

	_vec3		m_vMoveDirection = {};		// �̵��� or �Ĵ� �� ����
	_float		m_fFollowDistance = 10.f;	// ���� ������ �Ÿ�
	_float		m_fFollowLimitNear = 1.5f;
	_float		m_fAttackDistance = 1.5f;	// ���� ������ �Ÿ�


	//--------------------------------------------------
	// ����
	//--------------------------------------------------
	_bool		m_bCanAttack = true;		// ���� ���� or �Ұ���
	_float		m_fAttackDelay = 3.5f;		// ���� ������ ������
	_float		m_fAttackTimer = 0.f;		// ���� ��Ÿ�� �ð� ��¿�
	_float		m_fBloodCreateTime = 0.f;
	INSTANTIMPACT	m_tImpact = {};


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
	_int		m_iAnimStep = 0;
	_float		m_fAnimTimer = 0.f;
};

END

#endif // !__WOLF_H__

