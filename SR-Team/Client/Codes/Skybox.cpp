#include "stdafx.h"
#include "..\Headers\Skybox.h"

USING(Client)

CSkybox::CSkybox(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CSkybox::CSkybox(const CSkybox & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CSkybox::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkybox::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_iTextureID = (*(_uint*)_pArg);

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CSkybox::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CSkybox::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_PRIORITY, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CSkybox::Render_Priority()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;


	//사용할 매트릭스 설정

	_matrix IdentityMatrix /*ViewMatrix, ProjMatrix*/;

	D3DXMatrixIdentity(&IdentityMatrix);

	//Skybox전용 RenderSetting

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE)))
		return E_FAIL;


	// 버텍스 버퍼

	if (FAILED(((CVIBuffer_CubeTexture*)m_pVIBufferCom)->Set_Transform_Nothing(&m_pTransformCom->Get_Desc().matWorld)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, pCamera->Get_ViewMatrix())))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, pCamera->Get_ProjMatrix())))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(m_iTextureID)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;





	//m_pDevice 설정복구
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &IdentityMatrix)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &IdentityMatrix)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &IdentityMatrix)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE)))
		return E_FAIL;

	return S_OK;
}


HRESULT CSkybox::Add_Component()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Skybox", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORM_DESC));

	tTransformDesc.vScale = _vec3(100.f, 100.f, 100.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CSkybox::Movement(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	m_pTransformCom->Set_Position(pCamera->Get_Desc().vEye);

	return S_OK;
}

CSkybox * CSkybox::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkybox* pInstance = new CSkybox(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSkybox", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkybox::Clone_GameObject(void * _pArg)
{
	CSkybox* pInstance = new CSkybox(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkybox", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkybox::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
