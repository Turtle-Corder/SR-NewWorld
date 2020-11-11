#pragma once
#ifndef __GOLEM_DASH_H__
#define __GOLEM_DASH_H__

#include "GameObject.h"

	USING(Engine)
	BEGIN(Client)

class CGolem_Dash final : public CGameObject
{
private:
	explicit CGolem_Dash(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGolem_Dash(const CGolem_Dash& _rOther);
	virtual ~CGolem_Dash() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Particle_Move(_float _fDeltaTime);

public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	static CGolem_Dash* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;


private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;



	INSTANTIMPACT		m_tInstant = {};
	_float				m_fDeadTime = 0.f;
	_float				m_fTimeFlow = 0.f;
	_uint				m_iTexture_Number = 0;
};

END

#endif //__GOLEM_DASH_H__
