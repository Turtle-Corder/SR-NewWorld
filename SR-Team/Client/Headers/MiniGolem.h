#pragma once
#ifndef __MINI_GOLEM_H__
#define __MINI_GOLEM_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)
class CDamageInfo;

class CMiniGolem final : public CGameObject
{
	enum
	{
		MINIGOLEM_BASE,
		MINIGOLEM_BODY,
		MINIGOLEM_HEAD,
		MINIGOLEM_LEFT_ARM,
		MINIGOLEM_RIGHT_ARM,
		MINIGOLEM_LEFT_LEG,
		MINIGOLEM_RIGHT_LEG,
		MINIGOLEM_END
	};

private:
	explicit CMiniGolem(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMiniGolem(const CMiniGolem& _rOther);
	virtual ~CMiniGolem() = default;


	//------------------------------------------------------------
	// Function
	//------------------------------------------------------------
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

	virtual HRESULT Render_NoneAlpha() override;

	virtual void Free() override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	static  CMiniGolem* Create(LPDIRECT3DDEVICE9 _pDevice);

	HRESULT Add_Component();

private:
	//------------------------------------------------------------
	// Add Component
	//------------------------------------------------------------
	HRESULT Add_Component_VIBuffer();
	HRESULT Add_Component_Transform();
	HRESULT Add_Component_Texture();
	HRESULT Add_Component_Extends();



	//------------------------------------------------------------
	// Transformation 변경
	//------------------------------------------------------------
	HRESULT IsOnTerrain();

	void Update_Disappear(_float _fDeltaTime);


	//------------------------------------------------------------
	// 애니메이션
	//------------------------------------------------------------
	HRESULT Update_Animation_Attack(_float _fDeltaTime);


	//------------------------------------------------------------
	// 공격체
	//------------------------------------------------------------
	HRESULT Spawn_GolemImpact();



	//------------------------------------------------------------
	// Variable (변수)
	//------------------------------------------------------------
private:
	CVIBuffer*			m_pVIBufferCom[MINIGOLEM_END] = {nullptr, };
	CTransform*			m_pTransformCom[MINIGOLEM_END] = { nullptr, };
	CTexture*			m_pTextureCom[MINIGOLEM_END] = { nullptr, };
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

	INSTANTIMPACT		m_tInstant = {};

	_bool				m_bCanAttack = true;
	_float				m_fAttackTime = 0.f;

	_float				m_fDisappearTime = 0.f;
	_float				m_fDisappearDelay = 7.f;
};

END

#endif //__MINI_GOLEM_H__