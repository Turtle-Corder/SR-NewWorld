#pragma once
#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider final : public CComponent
{
public:
	typedef struct tagColliderDesc
	{
		_vec3	vPosition = { 0.f, 0.f, 0.f };
		_float	fRadius = 0.f;
	} COLLIDER_DESC;

private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSphereCollider(const CSphereCollider& _rOther);
	virtual ~CSphereCollider() = default;

public:
	const COLLIDER_DESC& Get_Desc() const;

public:
	HRESULT Setup_Component_Prototype() override;
	HRESULT Setup_Component(void* _pArg) override;

	HRESULT Update_Collider(const _vec3& vPosition);
	void Set_Radius(_float _fRadius);

public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;



private:
	COLLIDER_DESC	m_tColliderDesc;
};

END

#endif 
