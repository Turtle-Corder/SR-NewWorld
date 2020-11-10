#pragma once


#ifndef __EVENTTRIGGER_H__
#define __EVENTTRIGGER_H__

BEGIN(Client)
USING(Engine)


class CEventTrigger : public CGameObject
{
private:
	explicit CEventTrigger(LPDIRECT3DDEVICE9 pDevice);
	explicit CEventTrigger(const CEventTrigger& other);
	virtual ~CEventTrigger() = default;


	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;


private:
	virtual HRESULT Add_Component();

	HRESULT Floating(_float _fDeltaTime);

public:
	static CEventTrigger* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CSphereCollider*	m_pColliderCom = nullptr;

	_float		m_fTimeFlow = 0.f;

	EVENT_INFO	m_tInfo;
};

END

#endif
