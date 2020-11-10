#pragma once
#ifndef __BOMB_RESIDUE_H__
#define __BOMB_RESIDUE_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CBomb_Residue final : public CGameObject
{
private:
	explicit CBomb_Residue(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBomb_Residue(const CBomb_Residue& _rOther);
	virtual ~CBomb_Residue() = default;

	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Render_NoneAlpha() override;


private:
	HRESULT Add_Component();
	HRESULT Particle_Move(_float _fDeltaTime);

public:
	static CBomb_Residue* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);

private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;



	INSTANTIMPACT		m_tInstant = {};
	_float				m_fDeadTime = 0.f;
	_uint				m_iRandSpeed = 0;
	_float				m_fJumpPower = 0.f;
	_float				m_fJumpTime = 0.f;
};

END

#endif //__BOMB_RESIDUE_H__

