#pragma once
#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CPipeline final
{
private:
	CPipeline();
	~CPipeline() = default;

public:
	static HRESULT Setup_WorldMatrix(_matrix* _pOut, const _vec3& _vScale, const _vec3& _vRotate, const _vec3& _vPosition);
	static HRESULT Setup_ViewMatrix(_matrix* _pOut, const _vec3& _vEye, const _vec3& _vAt, const _vec3& _vUp);
	static HRESULT Setup_ProjectionMatrix(_matrix* _pOut, float _fFovY, float _fAspect, float _fNear, float _fFar);
	static HRESULT Setup_StateMatrix(_matrix* _pOut, const _vec3& _vRight, const _vec3& _vUp, const _vec3& _vLook, const _vec3& _vPosition);
	static HRESULT TransformCoord(_vec3* _pOut, const _vec3* _pIn, const _matrix* _pMat);

private:
	static HRESULT Rotate_AxisX(_vec3* _pOut, const _vec3& _vIn, float _fRadian);
	static HRESULT Rotate_AxisY(_vec3* _pOut, const _vec3& _vIn, float _fRadian);
	static HRESULT Rotate_AxisZ(_vec3* _pOut, const _vec3& _vIn, float _fRadian);
};

END

#endif
