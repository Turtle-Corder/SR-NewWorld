#include "stdafx.h"
#include "..\Headers\Terrain.h"

USING(Client)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CTerrain::CTerrain(const CTerrain & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CTerrain::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CTerrain::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CTerrain::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::ERR;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CTerrain::Render_NoneAlpha()
{
	/*CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
*/
	return S_OK;
}

HRESULT CTerrain::Add_Component()
{
	// UNDONE : expands
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_VIBuffer_TerrainTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CTerrain* pInstance = new CTerrain(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CTerrain", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone_GameObject(void * _pArg)
{
	CTerrain* pInstance = new CTerrain(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CTerrain", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
