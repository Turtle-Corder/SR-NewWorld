#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

BEGIN(Client)
USING(Engine)

class CStatus;
class CDamageInfo;
class CWand;
class CIceCrystal;
class CFireCrystal;

class CPlayer final : public CGameObject
{
public:
	enum STATE
	{
		IDLE,
		MOVE,
		VALIDATE_MOVE = MOVE,

		JUMP, 
		ATTACK, 
		SKILL,
		DEAD,
		STATE_END 
	};

	enum SLOT_SKILL
	{
		SLOT_Q,
		SLOT_W,
		SLOT_E,
		SLOT_R,
		SLOT_A,
		SLOT_S,
		SLOT_D,
		SLOT_F
	};

	enum SLOT_ITEM_QUEST
	{
		SLOT_F1,
		SLOT_F2,
		SLOT_F3,
		SLOT_F4
	};

	enum SLOT_ITEM_CONSUME
	{
		SLOT_1,
		SLOT_2,
		SLOT_3,
		SLOT_4
	};

	enum ACTIVE_BUFF 
	{ 
		BUFF_MANA, 
		BUFF_ATTACK, 
		BUFF_SHIELD, 
		BUFF_END 
	};



private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPlayer(const CPlayer& _rOther);
	virtual ~CPlayer() = default;


	//----------------------------------------------------------------------------------------------------
	// Common
	//----------------------------------------------------------------------------------------------------
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
	virtual HRESULT Render_BlendAlpha() override;
	virtual HRESULT Render_UI() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;

public:
	_bool IsOnBuff(ACTIVE_BUFF _eType);
	void Buff_On(ACTIVE_BUFF _eType);
	void Buff_Off(ACTIVE_BUFF _eType);

	_int Get_ClearInfo();
	HRESULT Set_ClearInfo(_int _iClearScene);
	
	void Set_ConsumeRate(_float _fConsumeRate);

	void Active_IceCrystal();
	void DeActive_IceCrystal();

	void Active_FireCrystal();
	void DeActive_FireCrystal();

	_bool IsInteraction();

private:
	//----------------------------------------------------------------------------------------------------
	// Add Component
	//----------------------------------------------------------------------------------------------------
	HRESULT Add_Component();

	HRESULT Add_Component_Transform();
	HRESULT Add_Component_VIBuffer();
	HRESULT Add_Component_Texture();
	HRESULT Add_Component_Extends();

	// 장비지만 여기 껴준다..
	HRESULT Add_Extends(const wstring & LayerTag);



	//----------------------------------------------------------------------------------------------------
	// UI
	//----------------------------------------------------------------------------------------------------
	_int Update_UICheck();



	//----------------------------------------------------------------------------------------------------
	// State
	//----------------------------------------------------------------------------------------------------
	_int Update_State();



	//----------------------------------------------------------------------------------------------------
	// Input
	//----------------------------------------------------------------------------------------------------
	_int Update_Input(_float _fDeltaTime);
	_int Update_Input_Skill(_float _fDeltaTime);
	_int Update_Input_Item(_float _fDeltaTime);
	_int Update_Input_Action(_float _fDeltaTime);

	_bool Actual_UseSkill();



	//----------------------------------------------------------------------------------------------------
	// Transform
	//----------------------------------------------------------------------------------------------------
	_int Update_Parts();

	//--------------------------------------------------
	// 이동
	//--------------------------------------------------
	HRESULT Update_Move(_float _fDeltaTime);
	HRESULT Update_OnTerrain();

	//--------------------------------------------------
	// 회전
	//--------------------------------------------------
	HRESULT Update_Look(_float _fDeltaTime);

	//--------------------------------------------------
	// 점프
	//--------------------------------------------------
//	void Update_Jump(_float fDeltaTime);

	//--------------------------------------------------
	// 마우스 피킹
	//--------------------------------------------------
	HRESULT Raycast_OnTerrain(_bool* _pFound, _vec3* _pPos);	
	HRESULT Raycast_OnMonster(_bool* _pFound, CGameObject** _ppObject);
	void Move_Target(_float _fDeltaTime);





	//----------------------------------------------------------------------------------------------------
	// Anim
	//----------------------------------------------------------------------------------------------------
	void Update_Anim(_float _fDeltaTime);

	void Update_Anim_Move(_float _fDeltaTime);
	void Update_Anim_Attack(_float _fDeltaTime);
	void Update_Anim_Skill(_float _fDeltaTime);

	void Update_Anim_Skill_Common(_float _fDeltaTime);


private:
	//----------------------------------------------------------------------------------------------------
	// 일반 공격
	//----------------------------------------------------------------------------------------------------
	HRESULT Spawn_EnergyBolt();

	void Update_AtkDelay(_float _fDeltaTime);

	void Update_FlinchDelay(_float _fDeltaTime);

	void Update_HurtDelay(_float _fDeltaTime);

	void Update_DecalAlpha(_float _fDelatTime);


private:
	//----------------------------------------------------------------------------------------------------
	// Variables
	//----------------------------------------------------------------------------------------------------
	
	//--------------------------------------------------
	// Components
	//--------------------------------------------------
	CVIBuffer*			m_pVIBufferCom[PART_END];
	CTransform*			m_pTransformCom[PART_END];
	CTexture*			m_pTextureCom[PART_END];
	CTexture*			m_pFlinchTexCom = nullptr;
	CTexture*			m_pDecalTexCom	= nullptr;
	CRaycast*			m_pRaycastCom	= nullptr;
	CSphereCollider*	m_pColliderCom	= nullptr;
	CStatus*			m_pStatusCom	= nullptr;
	CDamageInfo*		m_pDmgInfoCom	= nullptr;


	//--------------------------------------------------
	// State
	//--------------------------------------------------
	STATE				m_ePreState = IDLE;
	STATE				m_eCurState = IDLE;


	//--------------------------------------------------
	// State
	//--------------------------------------------------
	_float				m_fJumpTime		= 0.f;
	_float				m_fJumpPower	= 1.f;

	_vec3				m_vTargetPos	= {};

	_bool				m_bCanNormalAtk = true;
	_float				m_fAtkDelayTime = 0.5f;
	_float				m_fAtkDelayCounter = 0.f;

	_float				m_fNearDist = 0.3f;

	//--------------------------------------------------
	// UI
	//--------------------------------------------------
	_bool				bShowUI = false;


	//--------------------------------------------------
	// Animation
	//--------------------------------------------------
	_int				m_iAnimStep = 0;		// state 변경시 초기화 해 줌
	_float				m_fAnimTimer = 0.f;

	
	//--------------------------------------------------
	// Late Process
	//--------------------------------------------------
	_int				m_iInputIdx_Slot = -1;
	_int				m_iInputIdx_Anim = -1;


	//--------------------------------------------------
	// Buff
	//--------------------------------------------------
	_bool				m_bBuffActive[BUFF_END] = { false, };

	// mana, atk, shield rate
	_float				m_fConsumeRate = 1.f;


	//--------------------------------------------------
	// 공격체 생성시 셋팅할 구조체
	//--------------------------------------------------
	INSTANTIMPACT		m_tImpact = {};

	_int				m_iClearInfo = 0;


	//--------------------------------------------------
	// 블래스트 구체
	//--------------------------------------------------
	CIceCrystal*	m_pIceCrystal[3] = { nullptr, };
	_int			m_iActiveIceCrystal = 0;

	CFireCrystal*	m_pFireCrystal[3] = { nullptr };
	_int			m_iActiveFireCrystal = 0;

	_bool			m_bInteraction = false;


	//--------------------------------------------------
	// 피격
	//--------------------------------------------------
	_bool			m_bCanHurt = false;
	_float			m_fHurtTimer = 0.f;
	_float			m_fHurtDealy = 1.f;

	_bool			m_bFlinch = false;
	_float			m_fFlinchTimer = 0.f;
	_float			m_fFlinchDealy = 0.1f;

	_bool			m_bDecalOn = false;
	_bool			m_bDecalDir = false;
	_float			m_fCurDecalAlpha = 0.f;
	_float			m_fDecalAlpha = 20.f;
};

END

#endif // !__PLAYER_H__
