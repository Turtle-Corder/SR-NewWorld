#pragma once
#ifndef __BLOOD_H__
#define __BLOOD_H__
#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CBlood final : public CGameObject
{
private:
	explicit CBlood(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBlood(const CBlood& _rOther);
	virtual ~CBlood() = default;
public:
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
	static CBlood* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);


private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;



	INSTANTIMPACT		m_tInstant = {};
	_float				m_fDeadTime = 0.f;
};

END

#endif //__BLOOD_H__