#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_matrix matView;
		_vec3	vEye, vAt, vUp;

		_matrix matProj;
		_float	fFovY, fAspect, fNear, fFar;
	} CAMERA_DESC;

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCamera(const CCamera& _rOther);
	virtual ~CCamera() = default;

public:
	const _matrix* Get_ViewMatrix() const;
	const _matrix* Get_ProjMatrix() const;
	const _vec3& Get_Position() const;
	const CAMERA_DESC Get_Desc() { return m_tCameraDesc; }
public:
	virtual HRESULT Setup_GameObject_Prototype() = 0;
	virtual HRESULT Setup_GameObject(void* _pArg) = 0;
	virtual _int Update_GameObject(_float _fDeltaTime) = 0;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) = 0;

public:
	virtual CGameObject* Clone_GameObject(void* _pArg) = 0;
	virtual void Free() override;



protected:
	CAMERA_DESC m_tCameraDesc;
};

END

#endif
