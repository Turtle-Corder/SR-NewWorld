#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum AXIS_XYZ
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z
	};

	typedef struct tagTransformDesc
	{
		_vec3	vScale = { 1.f, 1.f, 1.f };
		_vec3	vRotate = { 0.f, 0.f, 0.f };
		_vec3	vPosition = { 0.f, 0.f, 0.f };
		_matrix matWorld;

		_float	fSpeedPerSecond = 0.f;
		_float	fRotatePerSecond = 0.f;
		_matrix matAxis;
	} TRANSFORM_DESC;


private:
	explicit CTransform(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTransform(const CTransform& _rOther);
	virtual ~CTransform() = default;

public:
	const TRANSFORM_DESC& Get_Desc() const;
	const _vec3 Get_Look() const;
	const _vec3 Get_Up() const;
	const _vec3 Get_Right() const;

public:
	void Set_Transform(const TRANSFORM_DESC& _rTransform);
	void Set_Position(const _vec3& _vPosition);
	void Set_WorldMatrix(const _matrix& _matWorld);
	void Set_Rotation(const _vec3& _vRotation);
	void Set_Scale(const _vec3& _vScale);


public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

public:
	HRESULT Update_Transform();
	HRESULT Update_Transform(const _matrix& _matRevolution);
	HRESULT Update_Transform(const _matrix& _matRevolution, const _matrix& _matParent);
	HRESULT Update_Transform_Rotate(const _matrix* _matRotate);
	void Move_Vertical(_float _fDeltaTime);
	void Move_Horizontal(_float _fDeltaTime);
	void Turn(AXIS_XYZ _eAxis, _float _fDeltaTime);


public:
	static CTransform* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;



private:
	TRANSFORM_DESC m_tTransformDesc;
};

END

#endif
