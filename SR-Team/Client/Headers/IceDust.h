#pragma once
#ifndef __ICEDUST_H__
#define __ICEDUST_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CIceDust final : public CGameObject
{
private:
	explicit CIceDust(LPDIRECT3DDEVICE9 _pDevice);
	explicit CIceDust(const CIceDust& _rOther);
	virtual ~CIceDust() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Floating(_float _fDeltaTime);

public:
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;
	static CIceDust* Create(LPDIRECT3DDEVICE9 _pDevice);



private:
	CVIBuffer*		m_pVIBufferComp = nullptr;
	CTransform*		m_pTransformComp = nullptr;
	CTexture*		m_pTextureComp = nullptr;

	_vec3			m_vInitPos = {};
	_int			m_iRandRot = 0;

	_float			m_fFloatTime = 0.f;
	_float			m_fLifeTimer = 0.f;
	_float			m_fLife = 0.f;
};

END

#endif // !__ICEDUST_H__
