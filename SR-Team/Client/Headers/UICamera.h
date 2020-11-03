#pragma once
#ifndef __UICAMERA_H__
#define __UICAMERA_H__

#include "Camera.h"

USING(Engine)
BEGIN(Client)

class CUICamera final : public CCamera
{
private:
	explicit CUICamera(LPDIRECT3DDEVICE9 _pDevice);
	explicit CUICamera(const CUICamera& _rOther);
	virtual ~CUICamera() = default;

public:
	void Set_Start(bool bStart) { m_bStart = bStart; }

public:
	HRESULT Set_UITransform();

public:
	// CCamera을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;

private:
	HRESULT Movement(float fDeltaTime);

public:
	static CUICamera* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	bool	m_bStart = true;
};

END

#endif
