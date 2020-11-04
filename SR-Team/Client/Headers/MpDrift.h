#pragma once
#ifndef __MP_DRIFT_H__
#define __MP_DRIFT_H__

#include "GameObject.h"
USING(Engine)
BEGIN(Client)

class CMpDrift final : public CGameObject
{
private:
	explicit CMpDrift(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMpDrift(const CMpDrift& _rOther);
	virtual ~CMpDrift() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_OnlyAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
public:
	static CMpDrift* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;

	bool			m_bDead = false;
	_vec3			m_vStartPos = {};
	_uint			m_iCurrFrame = 0;
	_uint			m_iMaxCnt = 0;
	_float			m_fDeadTime = 0.f;
};

END

#endif //__MP_DRIFT_H__
