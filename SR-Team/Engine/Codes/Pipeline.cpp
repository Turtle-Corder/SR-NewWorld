#include "..\Headers\Pipeline.h"

USING(Engine)

CPipeline::CPipeline()
{
}

HRESULT CPipeline::Setup_WorldMatrix(_matrix * _pOut, const _vec3 & _vScale, const _vec3 & _vRotate, const _vec3 & _vPosition)
{
	if (nullptr == _pOut)
		return E_FAIL;

	D3DXMatrixIdentity(_pOut);
	_vec3 vRight	= { 1.f, 0.f, 0.f };
	_vec3 vUp		= { 0.f, 1.f, 0.f };
	_vec3 vLook		= { 0.f, 0.f, 1.f };

	vRight.x *= _vScale.x;
	vUp.y *= _vScale.y;
	vLook.z *= _vScale.z;

	// x-axis
	Rotate_AxisX(&vRight, vRight, _vRotate.x);
	Rotate_AxisX(&vUp, vUp, _vRotate.x);
	Rotate_AxisX(&vLook, vLook, _vRotate.x);

	// y-axis
	Rotate_AxisY(&vRight, vRight, _vRotate.y);
	Rotate_AxisY(&vUp, vUp, _vRotate.y);
	Rotate_AxisY(&vLook, vLook, _vRotate.y);

	// z-axis
	Rotate_AxisZ(&vRight, vRight, _vRotate.z);
	Rotate_AxisZ(&vUp, vUp, _vRotate.z);
	Rotate_AxisZ(&vLook, vLook, _vRotate.z);

	return Setup_StateMatrix(_pOut, vRight, vUp, vLook, _vPosition);
}

HRESULT CPipeline::Setup_WorldMatrix(_matrix * _pOut, const _vec3 & _vScale, const _vec3 & _vRotate, const _vec3 & _vPosition, const _matrix * _Rotation)
{
	if (nullptr == _pOut)
		return E_FAIL;

	D3DXMatrixIdentity(_pOut);
	_vec3 vRight = { 1.f, 0.f, 0.f };
	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vLook = { 0.f, 0.f, 1.f };

	vRight.x *= _vScale.x;
	vUp.y *= _vScale.y;
	vLook.z *= _vScale.z;

	// x-axis
	Rotate_AxisX(&vRight, vRight, _vRotate.x);
	Rotate_AxisX(&vUp, vUp, _vRotate.x);
	Rotate_AxisX(&vLook, vLook, _vRotate.x);

	// y-axis
	Rotate_AxisY(&vRight, vRight, _vRotate.y);
	Rotate_AxisY(&vUp, vUp, _vRotate.y);
	Rotate_AxisY(&vLook, vLook, _vRotate.y);

	// z-axis
	Rotate_AxisZ(&vRight, vRight, _vRotate.z);
	Rotate_AxisZ(&vUp, vUp, _vRotate.z);
	Rotate_AxisZ(&vLook, vLook, _vRotate.z);

	D3DXVec3TransformCoord(&vRight, &vRight, _Rotation);
	D3DXVec3TransformCoord(&vUp, &vUp, _Rotation);
	D3DXVec3TransformCoord(&vLook, &vLook, _Rotation);

	return Setup_StateMatrix(_pOut, vRight, vUp, vLook, _vPosition);
}

HRESULT CPipeline::Setup_ViewMatrix(_matrix * _pOut, const _vec3 & _vEye, const _vec3 & _vAt, const _vec3 & _vUp)
{
	if (nullptr == _pOut)
		return E_FAIL;

	D3DXMatrixIdentity(_pOut);

	_vec3 vRight	= { 1.f, 0.f, 0.f };
	_vec3 vRealUp	= { 0.f, 1.f, 0.f };
	_vec3 vLook		= { 0.f, 0.f, 1.f };
	_vec3 vPosition = _vEye;

	vLook = _vAt - _vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(&vRight, &_vUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vRealUp, &vLook, &vRight);
	D3DXVec3Normalize(&vRealUp, &vRealUp);

	if (FAILED(Setup_StateMatrix(_pOut, vRight, vRealUp, vLook, _vEye)))
	{
		PRINT_LOG(L"Failed To Setup_StateMatrix", LOG::ENGINE);
		return E_FAIL;
	}

	if (!D3DXMatrixInverse(_pOut, 0, _pOut))
	{
		PRINT_LOG(L"Failed To D3DXMatrixInverse", LOG::ENGINE);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPipeline::Setup_ProjectionMatrix(_matrix * _pOut, float _fFovY, float _fAspect, float _fNear, float _fFar)
{
	if (nullptr == _pOut)
		return E_FAIL;

	D3DXMatrixIdentity(_pOut);

	float fScale = 1.f / tanf(_fFovY * 0.5f);

	_pOut->_11 = fScale / _fAspect;
	_pOut->_22 = fScale;

	_pOut->_33 = _fFar / (_fFar - _fNear);
	_pOut->_43 = -(_fNear * _fFar) / (_fFar - _fNear);

	_pOut->_34 = 1.f;
	_pOut->_44 = 0.f;

	return S_OK;
}

HRESULT CPipeline::Setup_StateMatrix(_matrix * _pOut, const _vec3 & _vRight, const _vec3 & _vUp, const _vec3 & _vLook, const _vec3& _vPosition)
{
	if (nullptr == _pOut)
		return E_FAIL;

	D3DXMatrixIdentity(_pOut);

	memcpy_s(&_pOut->m[0][0], sizeof(_vec3), &_vRight, sizeof(_vec3));
	memcpy_s(&_pOut->m[1][0], sizeof(_vec3), &_vUp, sizeof(_vec3));
	memcpy_s(&_pOut->m[2][0], sizeof(_vec3), &_vLook, sizeof(_vec3));
	memcpy_s(&_pOut->m[3][0], sizeof(_vec3), &_vPosition, sizeof(_vec3));

	return S_OK;;
}

HRESULT CPipeline::TransformCoord(_vec3 * _pOut, const _vec3 * _pIn, const _matrix * _pMat)
{
	if (nullptr == _pOut)
		return E_FAIL;

	_vec4 vTemp = _vec4(_pIn->x, _pIn->y, _pIn->z, 1.f);
	
	_pOut->x = vTemp.x * _pMat->_11 + vTemp.y * _pMat->_21 + vTemp.z * _pMat->_31 + vTemp.w * _pMat->_41;
	_pOut->y = vTemp.x * _pMat->_12 + vTemp.y * _pMat->_22 + vTemp.z * _pMat->_32 + vTemp.w * _pMat->_42;
	_pOut->z = vTemp.x * _pMat->_13 + vTemp.y * _pMat->_23 + vTemp.z * _pMat->_33 + vTemp.w * _pMat->_43;

	_float fOldZ = vTemp.x * _pMat->_14 + vTemp.y * _pMat->_24 + vTemp.z * _pMat->_34 + vTemp.w * _pMat->_44;

	(*_pOut) /= fOldZ;

	return S_OK;
}

HRESULT CPipeline::Rotate_AxisX(_vec3 * _pOut, const _vec3 & _vIn, float _fRadian)
{
	if (nullptr == _pOut)
		return E_FAIL;

	_vec3 vTemp = _vIn;

	_pOut->y = vTemp.y * cosf(_fRadian) - vTemp.z * sinf(_fRadian);
	_pOut->z = vTemp.y * sinf(_fRadian) + vTemp.z * cosf(_fRadian);

	return S_OK;
}

HRESULT CPipeline::Rotate_AxisY(_vec3 * _pOut, const _vec3 & _vIn, float _fRadian)
{
	if (nullptr == _pOut)
		return E_FAIL;

	_vec3 vTemp = _vIn;

	_pOut->x = vTemp.x * cosf(_fRadian) + vTemp.z * sinf(_fRadian);
	_pOut->z = vTemp.x * -sinf(_fRadian) + vTemp.z * cosf(_fRadian);

	return S_OK;
}

HRESULT CPipeline::Rotate_AxisZ(_vec3 * _pOut, const _vec3 & _vIn, float _fRadian)
{
	if (nullptr == _pOut)
		return E_FAIL;

	_vec3 vTemp = _vIn;

	_pOut->x = vTemp.x * cosf(_fRadian) - vTemp.y * sinf(_fRadian);
	_pOut->y = vTemp.x * sinf(_fRadian) + vTemp.y * cosf(_fRadian);

	return S_OK;
}
