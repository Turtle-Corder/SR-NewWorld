#include "..\Headers\Raycast.h"

USING(Engine)

CRaycast::CRaycast(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CRaycast::CRaycast(const CRaycast & _rOther)
	: CComponent(_rOther)
	, m_vRayPos(_rOther.m_vRayPos)
	, m_vRayDir(_rOther.m_vRayDir)
{
}

HRESULT CRaycast::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CRaycast::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CRaycast::Convert_ViewportToLocal(HWND _hWnd, _uint _iWinCX, _uint _iWinCY, _matrix * _pMatWorld, CCamera * _pCamera)
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(_hWnd, &ptMouse);


	//--------------------------------------------------
	// viewport -> proj
	//--------------------------------------------------
	_vec3 vMouse = {};
	vMouse.x = ptMouse.x / (_iWinCX * 0.5f) - 1.f;
	vMouse.y = 1.f - ptMouse.y / (_iWinCY * 0.5f);
	vMouse.z = 0.f;


	//--------------------------------------------------
	// proj -> view
	//--------------------------------------------------
	const _matrix* pMatProj = _pCamera->Get_ProjMatrix();
	if (nullptr == pMatProj)
		return E_FAIL;

	_matrix matInvProj;
	D3DXMatrixInverse(&matInvProj, 0, pMatProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matInvProj);

	//--------------------------------------------------
	// view -> world
	//--------------------------------------------------
	m_vRayPos = { 0.f, 0.f, 0.f };
	m_vRayDir = vMouse - m_vRayPos;

	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	const _matrix* pMatView = _pCamera->Get_ViewMatrix();
	if (nullptr == pMatView)
		return E_FAIL;

	_matrix matInvView;
	D3DXMatrixInverse(&matInvView, 0, pMatView);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matInvView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matInvView);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	//--------------------------------------------------
	// world -> local
	//--------------------------------------------------
	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, _pMatWorld);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matInvWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matInvWorld);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	return S_OK;
}

CRaycast * CRaycast::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CRaycast* pInstance = new CRaycast(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CRaycast", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRaycast::Clone_Component(void * _pArg)
{
	CRaycast* pInstance = new CRaycast(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CRaycast", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaycast::Free()
{
	CComponent::Free();
}
