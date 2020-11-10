#include "stdafx.h"
#include "..\Headers\CubeTerrain.h"

USING(Client)

CCubeTerrain::CCubeTerrain(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CCubeTerrain::CCubeTerrain(const CCubeTerrain & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CCubeTerrain::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCubeTerrain::Setup_GameObject(void * _pArg)
{
	if(_pArg)
		m_tInfo = (*(TERRAININFO*)(_pArg));

	if (FAILED(Add_Component()))
	{
		PRINT_LOG(_T("CubeTerrain Setup Failed"), SCENE_STAGE0);
		return E_FAIL;
	}


	return S_OK;
}

_int CCubeTerrain::Update_GameObject(_float _fDeltaTime)
{
	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

_int CCubeTerrain::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::ERR;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CCubeTerrain::Render_NoneAlpha()
{
	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;


	_vec3 VecCameraToPos, VecCamera;
	D3DXVec3Normalize(&VecCameraToPos, &(m_pTransformCom->Get_Desc().vPosition - pCamera->Get_Position()));
	D3DXVec3Normalize(&VecCamera, &(pCamera->Get_Desc().vAt - pCamera->Get_Desc().vEye));

	if (cos(pCamera->Get_Desc().fFovY) < D3DXVec3Dot(&VecCameraToPos, &VecCamera))
	{
		if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom->SetTexture(m_tInfo.iTextureID)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CCubeTerrain::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = { m_tInfo.iX_Index * 2.f, m_tInfo.iFloor * 2.f - 0.99f,  m_tInfo.iZ_Index * 2.f };
	tTransformDesc.vScale *= 2.f;
	tTransformDesc.fSpeedPerSecond = 0.f;
	tTransformDesc.fRotatePerSecond = 0.f;



	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, _T("Component_VIBuffer_CubeTexture"), L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, _T("Component_Texture_TerrainBox"), L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, _T("Component_Transform"), L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	return S_OK;
}

CCubeTerrain * CCubeTerrain::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CCubeTerrain* pInstance = new CCubeTerrain(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMonster", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCubeTerrain::Clone_GameObject(void * _pArg)
{
	CCubeTerrain* pInstance = new CCubeTerrain(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMonster", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeTerrain::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
