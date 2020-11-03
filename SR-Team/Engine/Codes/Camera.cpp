#include "Pipeline.h"
#include "..\Headers\Camera.h"

USING(Engine)

CCamera::CCamera(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CCamera::CCamera(const CCamera & _rOther)
	: CGameObject(_rOther)
{
}

const _matrix * CCamera::Get_ViewMatrix() const
{
	return &m_tCameraDesc.matView;
}

const _matrix * CCamera::Get_ProjMatrix() const
{
	return &m_tCameraDesc.matProj;
}

const _vec3 & CCamera::Get_Position() const
{
	return m_tCameraDesc.vEye;
}

HRESULT CCamera::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Setup_GameObject(void * _pArg)
{
	return S_OK;
}

_int CCamera::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CCamera::LateUpdate_GameObject(_float _fDeltaTime)
{
	CPipeline::Setup_ViewMatrix(
		&m_tCameraDesc.matView
		, m_tCameraDesc.vEye
		, m_tCameraDesc.vAt
		, m_tCameraDesc.vUp);

	CPipeline::Setup_ProjectionMatrix(
		&m_tCameraDesc.matProj
		, m_tCameraDesc.fFovY
		, m_tCameraDesc.fAspect
		, m_tCameraDesc.fNear
		, m_tCameraDesc.fFar);

	return GAMEOBJECT::NOEVENT;
}

CGameObject * CCamera::Clone_GameObject(void * _pArg)
{
	return nullptr;
}

void CCamera::Free()
{
	CGameObject::Free();
}
