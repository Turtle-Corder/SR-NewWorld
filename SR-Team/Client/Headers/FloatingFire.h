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
	virtual HRESULT Setup_GameObject(void* _pArg) override;
	virtual int Update_GameObject(_float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_OnlyAlpha() override;
	virtual HRESULT Render_BlendAlpha() override;
private:
	HRESULT Add_Component();
	void Update_DeadDelay(_float _fDeltaTime);
	void Update_Scale(_float _fDeltaTime);

public:
	static CFloatingFire* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;

	INSTANTIMPACT	m_tImpact = {};
	_uint			m_iCurrFrame = 0;
	_float			m_fDeadTimer = 0.f;
	_float			m_fDeadDelay = 10.f;

	_float			m_fScaleMin = 2.f;
	_float			m_fScaleMax = 4.f;
	_bool			m_bScaleUp = true;

	_float			m_fScaleTimer = 0.f;
	_float			m_fScaleSpeed = 1.f;
};

END

#endif // !__EXPLOSION_H__
