#pragma once
#ifndef __DAMAGEFLOAT_H__
#define __DAMAGEFLOAT_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageFloat final : public CGameObject
{
private:
	explicit CDamageFloat(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDamageFloat(CONST CDamageFloat& _rOther);
	virtual ~CDamageFloat() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha();

	void Set_FloatValue(_int _iFloatValue);

private:
	HRESULT Add_Component();

public:
	static CDamageFloat* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*			m_pVIBufferCom[4];
	CTransform*			m_pTransformCom[4];
	CTexture*			m_pTextureCom = nullptr;

	FLOATING_INFO		m_tInfo = {};
	_float				m_fDeadTimer = 0.f;
	_int				m_iFloatValue = 0;

	_int				m_iMaxDigit = 0;
	_int				m_iDigitNum[4] = { 0, };
};

END

#endif // !__DAMAGEFLOAT_H__
