#pragma once
#ifndef __FLOATINGFIRE_H__
#define __FLOATINGFIRE_H__

USING(Engine)
BEGIN(Client)

class CFloatingFire final : public CGameObject
{
private:
	explicit CFloatingFire(LPDIRECT3DDEVICE9 _pDevice);
	explicit CFloatingFire(const CFloatingFire& _rOther);
	virtual ~CFloatingFire() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_BlendAlpha() override;


private:
	HRESULT Add_Component();
	void Update_DeadDelay(_float _fDeltaTime);

public:
	static CFloatingFire* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*	m_pVIBufferComp = nullptr;
	CTransform*	m_pTransformComp = nullptr;
	CTexture*	m_pTextureComp = nullptr;

	bool			m_bDead = false;
	INSTANTIMPACT	m_tImpact = {};
	_vec3			m_vDir = {};

	_uint			m_iCurrFrame = 0;
	_float			m_fDeadTimer = 0.f;
	_float			m_fDeadDelay = 10.f;
	_float			m_fAngle = 0.f;
	_float			m_fCircleRange = 0.f;
	_float			m_fInitDelay = 10.f;
	_float			m_fInitTimer = 0.f;
};

END

#endif // !__EXPLOSION_H__
