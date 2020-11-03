#include "stdafx.h"
#include "Pipeline.h"
#include "..\Headers\UICamera.h"

USING(Client)

CUICamera::CUICamera(LPDIRECT3DDEVICE9 _pDevice)
	: CCamera(_pDevice)
{
}

CUICamera::CUICamera(const CUICamera & _rOther)
	: CCamera(_rOther)
{
}

HRESULT CUICamera::Set_UITransform()
{
	// 월드행렬을 항등행렬로
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	// 뷰행렬을 항등행렬로
	D3DXMatrixIdentity(&m_tCameraDesc.matView);

	// 카메라로부터 직교투영행렬 얻기
	D3DXMatrixIdentity(&m_tCameraDesc.matProj);

	m_tCameraDesc.matView._11 = 210.f;
	m_tCameraDesc.matView._22 = 222.f;
	m_tCameraDesc.matView._33 = 1.f;

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_tCameraDesc.matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_tCameraDesc.matProj);

	return S_OK;
}

HRESULT CUICamera::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CUICamera::Setup_GameObject(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	memcpy_s(&m_tCameraDesc, sizeof(CCamera::CAMERA_DESC), pArg, sizeof(CCamera::CAMERA_DESC));

	return S_OK;
}

int CUICamera::Update_GameObject(float DeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

int CUICamera::LateUpdate_GameObject(float DeltaTime)
{
	// 뷰행렬은 항등행렬
	D3DXMatrixIdentity(&m_tCameraDesc.matView);

	// 직교투영행렬 만드는 함수
	D3DXMatrixOrthoLH(&m_tCameraDesc.matProj, WINCX, WINCY, 0.f, 1.f);
	//D3DXMatrixOrthoLH(&m_CameraDesc.matProj, 800.f, 600.f, 1.f, 500.f);

	return GAMEOBJECT::NOEVENT;
}

HRESULT CUICamera::Movement(float fDeltaTime)
{
	return S_OK;
}

CUICamera * CUICamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CUICamera* pInstance = new CUICamera(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CUICamera", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUICamera::Clone_GameObject(void * pArg)
{
	CUICamera* pInstance = new CUICamera(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CUICamera", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUICamera::Free()
{
	CCamera::Free();
}
