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

	enum EVENT_NUM
	{
		PORTAL, TWODIMENVIEW, THREEDIMENVIEW, EVENT_END
	};


private:
	virtual HRESULT Add_Component();
	HRESULT Add_Component(_vec3 _vPos);

	HRESULT Collision_Check();

public:
	static CEventTrigger* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CSphereCollider*  m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCollider = nullptr;

	EVENT_NUM m_eEventName;

};




END

#endif
