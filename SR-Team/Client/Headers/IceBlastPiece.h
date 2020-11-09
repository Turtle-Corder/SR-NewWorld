#pragma once
#ifndef __ICEPIECE_H__
#define __ICEPIECE_H__

#include"GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDamageInfo;

class CIceBlastPiece final : public CGameObject
{
private:
	explicit CIceBlastPiece(LPDIRECT3DDEVICE9 pDevice);
	explicit CIceBlastPiece(const CIceBlastPiece& other);
	virtual ~CIceBlastPiece() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;
	virtual int Update_GameObject(_float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT ParticleMove(_float _fDeltaTime);

public:
	static CIceBlastPiece* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;


private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	_float FlyingTime = 2.f;
	_float m_fDeadTime = 0.f;
	_vec3 m_vMoveDir = {};

	INSTANTIMPACT m_tInstant = {};
};

END

#endif // !__ICEPIECE_H__
