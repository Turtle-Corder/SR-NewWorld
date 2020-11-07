#pragma once
#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

USING(Engine)
BEGIN(Client)

class CExplosion final : public CGameObject
{
private:
	explicit CExplosion(LPDIRECT3DDEVICE9 _pDevice);
	explicit CExplosion(const CExplosion& _rOther);
	virtual ~CExplosion() = default;
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
	void Update_DeadDelay(float _fDeltaTime);

public:
	static CExplosion* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;

	bool			m_bDead = false;
	INSTANTIMPACT	m_tImpact = {};
	_uint			m_iCurrFrame = 0;
	_float			m_fDeadTimer = 0.f;
	_float			m_fDeadDelay = 10.f;
};

END

#endif // !__EXPLOSION_H__
