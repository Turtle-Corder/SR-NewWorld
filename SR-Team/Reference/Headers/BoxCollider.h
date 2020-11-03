#pragma once
#ifndef __AABBCOLLIDER_H__
#define __AABBCOLLIDER_H__

#include "Component.h"

BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CBoxCollider final : public CComponent
{

public:
	typedef struct tagBoxColliderDesc
	{
		_vec3	vPosition = { 0.f, 0.f, 0.f };
		_vec3	vObjectSize = { 1.f, 1.f, 1.f };
	} BOXCOLLIDER_DESC;


private:
	explicit CBoxCollider(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBoxCollider(const CBoxCollider& _rOther);
	virtual ~CBoxCollider() = default;

public:
	const BOXCOLLIDER_DESC& Get_Desc() const;

public:
	HRESULT Setup_Component_Prototype() override;
	HRESULT Setup_Component(void* _pArg) override;

	HRESULT Update_Collider(const _vec3& vPosition);
	void Set_Size(_vec3 _vSize);

public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;



private:
	BOXCOLLIDER_DESC	m_tColliderDesc;
};

END

#endif