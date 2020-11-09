#pragma once
#ifndef __ICEBLAST_H__
#define __ICEBLAST_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDamageInfo;
class CIceBlast final : public CGameObject
{
private:
	explicit CIceBlast(LPDIRECT3DDEVICE9 _pDevice);
	explicit CIceBlast(const CIceBlast& _rOther);
	virtual ~CIceBlast() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();

	void Update_InitDelay(_float _fDeltaTime);
	void Update_DeadDelay(_float _fDeltaTime);
	HRESULT Make_Pieces();
public:
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;
	static CIceBlast* Create(LPDIRECT3DDEVICE9 _pDevice);



private:
	CVIBuffer*		m_pVIBufferComp = nullptr;
	CTransform*		m_pTransformComp = nullptr;
	CTexture*		m_pTextureComp = nullptr;

	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusComp = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;
	
	// damage, collider, status

	INSTANTIMPACT	m_tImpact;

	_float			m_fInitTimer = 0.f;
	_float			m_fInitDelay = 1.f;
	_float			m_fScale = 0.f;
	_float			m_fDeadTimer = 0.f;
	_float			m_fDeadDelay = 5.f;
};

END

#endif // !__ICEBLAST_H__
