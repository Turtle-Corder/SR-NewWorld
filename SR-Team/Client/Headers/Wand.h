#pragma once
#ifndef __WAND_H__
#define __WAND_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CWand final : public CGameObject
{
	enum WAND
	{ 
		WAND_BASE , 
		WAND_HANDLE,
		WAND_HEAD,
		WAND_END
	};
	enum STATE
	{
		IDLE , NORMAL_ATTACK , SKILL_ATTACK 
	};
private:
	explicit CWand(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWand(const CWand& _rOther);
	virtual ~CWand() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT	Setting_Handle();
	HRESULT Setting_Head();
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static CWand* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);
	HRESULT	Change_State(STATE _eState);
private:
	CVIBuffer*			m_pVIBufferCom[WAND_END] = {};
	CTransform*			m_pTransformCom[WAND_END] = {};
	CTexture*			m_pTextureCom[WAND_END] = {};
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusComp = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;

	INSTANTIMPACT		m_tInstant = {};
	STATE				m_eState = IDLE;
	_vec3				m_vPlayer_RightHand_Pos = {};
	_float				m_fAngle = 45.f;
	vector<_matrix>		m_Test = {};
	vector<_vec3>		m_vTest = {};
};

END

#endif //__WAND_H__
