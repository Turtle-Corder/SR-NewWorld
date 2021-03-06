#pragma once
#ifndef __ACTUAL_BLIND_H__
#define __ACTUAL_BLIND_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CBlind final : public CGameObject
{
private:
	explicit CBlind(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBlind(const CBlind& _rOther);
	virtual ~CBlind() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Render_BlendAlpha() override;

public:
	static CBlind* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;

private:
	HRESULT Add_Component();
	void Update_DeadDelay(_float _fDeltaTime);
	void Update_TargetDeadCheck();



private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;


	_bool				m_bDead = false;
	INSTANTIMPACT		m_tImpact = {};
	_float				m_fMoveTime = 0.f;
	_float				m_fDeadTime = 0.f;
};

END

#endif // !__BLIND_H__
