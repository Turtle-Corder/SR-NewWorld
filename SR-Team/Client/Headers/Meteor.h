#pragma once
#ifndef __METEOR_H__
#define __METEOR_H__

#include"GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDamageInfo;

class CMeteor final : public CGameObject
{
private:
	explicit CMeteor(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeteor(const CMeteor& other);
	virtual ~CMeteor() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;
	virtual int Update_GameObject(_float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT FallDown_Meteor(_float _fDeltaTime);

public:
	static CMeteor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);


private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;
	CStatus*			m_pStatusComp = nullptr;
	CDamageInfo*		m_pDmgInfoCom = nullptr;
	
	_float m_fDeadTime = 0.f;
	_vec3 m_vMoveDir = {};

	INSTANTIMPACT m_tInstant = {};
};

END

#endif // !__METEOR_H__
