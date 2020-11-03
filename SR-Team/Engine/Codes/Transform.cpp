#include "Pipeline.h"
#include "..\Headers\Transform.h"

USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CTransform::CTransform(const CTransform & _rOther)
	: CComponent(_rOther)
	, m_tTransformDesc(_rOther.m_tTransformDesc)
{
}

const CTransform::TRANSFORM_DESC & CTransform::Get_Desc() const
{
	return m_tTransformDesc;
}

const _vec3 CTransform::Get_Look() const
{
	return *(_vec3*)&m_tTransformDesc.matWorld.m[2][0];
}

const _vec3 CTransform::Get_Up() const
{
	return *(_vec3*)&m_tTransformDesc.matWorld.m[1][0];
}

const _vec3 CTransform::Get_Right() const
{
	return *(_vec3*)&m_tTransformDesc.matWorld.m[0][0];
}

void CTransform::Set_Transform(const TRANSFORM_DESC & _rTransform)
{
	m_tTransformDesc = _rTransform;
}

void CTransform::Set_Position(const _vec3 & _vPosition)
{
	m_tTransformDesc.vPosition = _vPosition;
}

void CTransform::Set_WorldMatrix(const _matrix & _matWorld)
{
	m_tTransformDesc.matWorld = _matWorld;
}

void CTransform::Set_Rotation(const _vec3 & _vRotation)
{
	m_tTransformDesc.vRotate = _vRotation;
}

void CTransform::Set_Scale(const _vec3 & _vScale)
{
	m_tTransformDesc.vScale = _vScale;
}

HRESULT CTransform::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Setup_Component(void * _pArg)
{
	if (nullptr != _pArg)
		memcpy_s(&m_tTransformDesc, sizeof(TRANSFORM_DESC), _pArg, sizeof(TRANSFORM_DESC));

	return S_OK;
}

HRESULT CTransform::Update_Transform()
{
	return CPipeline::Setup_WorldMatrix(
		&m_tTransformDesc.matWorld,
		m_tTransformDesc.vScale,
		m_tTransformDesc.vRotate,
		m_tTransformDesc.vPosition);
}

HRESULT CTransform::Update_Transform(const _matrix & _matRevolution)
{
	if (FAILED(CPipeline::Setup_WorldMatrix(
		&m_tTransformDesc.matWorld,
		m_tTransformDesc.vScale,
		m_tTransformDesc.vRotate,
		m_tTransformDesc.vPosition)))
		return E_FAIL;

	m_tTransformDesc.matWorld = m_tTransformDesc.matWorld * _matRevolution;

	return S_OK;
}

HRESULT CTransform::Update_Transform(const _matrix & _matRevolution, const _matrix & _matParent)
{
	if (FAILED(CPipeline::Setup_WorldMatrix(
		&m_tTransformDesc.matWorld,
		m_tTransformDesc.vScale,
		m_tTransformDesc.vRotate,
		m_tTransformDesc.vPosition)))
		return E_FAIL;

	m_tTransformDesc.matWorld = m_tTransformDesc.matWorld * _matRevolution * _matParent;

	return S_OK;
}

void CTransform::Move_Vertical(_float _fDeltaTime)
{
	_vec3 vLook;
	memcpy_s(&vLook, sizeof(_vec3), &m_tTransformDesc.matWorld.m[2][0], sizeof(_vec3));

	D3DXVec3Normalize(&vLook, &vLook);
	m_tTransformDesc.vPosition += vLook * m_tTransformDesc.fSpeedPerSecond * _fDeltaTime;
}

void CTransform::Move_Horizontal(_float _fDeltaTime)
{
	_vec3 vRight;
	memcpy_s(&vRight, sizeof(_vec3), &m_tTransformDesc.matWorld.m[0][0], sizeof(_vec3));

	D3DXVec3Normalize(&vRight, &vRight);
	m_tTransformDesc.vPosition += vRight * m_tTransformDesc.fSpeedPerSecond * _fDeltaTime;
}

void CTransform::Turn(AXIS_XYZ _eAxis, _float _fDeltaTime)
{
	switch (_eAxis)
	{
	case Engine::CTransform::AXIS_X:
		m_tTransformDesc.vRotate.x += m_tTransformDesc.fRotatePerSecond * _fDeltaTime;
		break;

	case Engine::CTransform::AXIS_Y:
		m_tTransformDesc.vRotate.y += m_tTransformDesc.fRotatePerSecond * _fDeltaTime;
		break;

	case Engine::CTransform::AXIS_Z:
		m_tTransformDesc.vRotate.z += m_tTransformDesc.fRotatePerSecond * _fDeltaTime;
		break;
	}
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CTransform* pInstance = new CTransform(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CTransform", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone_Component(void * _pArg)
{
	CTransform* pInstance = new CTransform(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CTransform", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	CComponent::Free();
}
