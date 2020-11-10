#pragma once
#ifndef __RUBBLE_H__
#define __RUBBLE_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CRubble final : public CGameObject
{
private:
	explicit CRubble(LPDIRECT3DDEVICE9 _pDevice);
	explicit CRubble(const CRubble& _rOther);
	virtual ~CRubble() = default;

	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Render_NoneAlpha() override;


private:
	HRESULT Add_Component();
	HRESULT Particle_Move(_float _fDeltaTime);

public:
	static CRubble* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);


private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;



	INSTANTIMPACT		m_tInstant = {};
	_float				m_fDeadTime = 0.f;
	_float				m_fJumpPower = 0.f;
	_float				m_fJumpTime = 0.f;
};

END

#endif //__RUBBLE_H__
