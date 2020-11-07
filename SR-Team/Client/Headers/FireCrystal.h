#pragma once
#ifndef __FIRECRYSTAL_H__
#define __FIRECRYSTAL_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CFireCrystal final : public CGameObject
{
private:
	explicit CFireCrystal(LPDIRECT3DDEVICE9 _pDevice);
	explicit CFireCrystal(const CFireCrystal& _rOther);
	virtual ~CFireCrystal() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();

public:
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;
	static CFireCrystal* Create(LPDIRECT3DDEVICE9 _pDevice);



private:
	CVIBuffer*		m_pVIBufferComp = nullptr;
	CTransform*		m_pTransformComp = nullptr;
	CTexture*		m_pTextureComp = nullptr;

	_vec3			m_vInitPos = {};
	_int			m_iRandRot = 0;
};

END

#endif