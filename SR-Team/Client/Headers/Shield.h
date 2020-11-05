#pragma once
#ifndef __SHIELD_H__
#define __SHIELD_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CShield final : public CGameObject
{
	enum SHIELD
	{
		SHIELD_BASE,
		SHIELD_MAIN,
		SHIELD_LEFT,
		SHIELD_RIGHT,
		SHIELD_END
	};
private:
	explicit CShield(LPDIRECT3DDEVICE9 _pDevice);
	explicit CShield(const CShield& _rOther);
	virtual ~CShield() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Render_BlendAlpha() override;
public:
	static CShield* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
private:
	CVIBuffer*			m_pVIBufferCom[SHIELD_END] = {};
	CTransform*			m_pTransformCom[SHIELD_END] = {};
	CTexture*			m_pTextureCom[SHIELD_END] = {};


	_bool				m_bDead = false;
	_bool				m_bShieldMain_RenderCheck = false;
	INSTANTIMPACT		m_tInstant = {};
	_float				m_fMoveTime = 0.f;
	_float				m_fDeadTime = 0.f;

};

END

#endif //__SHIELD_H__