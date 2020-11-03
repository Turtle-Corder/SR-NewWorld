#include "stdafx.h"
#include "..\Headers\DummyTerrain.h"

USING(Client)

CDummyTerrain::CDummyTerrain(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CDummyTerrain::CDummyTerrain(const CDummyTerrain & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CDummyTerrain::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CDummyTerrain::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CDummyTerrain::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CDummyTerrain::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::ERR;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CDummyTerrain::Render_NoneAlpha()
{
	return S_OK;
}

HRESULT CDummyTerrain::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_TerrainTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

CDummyTerrain * CDummyTerrain::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CDummyTerrain* pInstance = new CDummyTerrain(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CTerrain", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDummyTerrain::Clone_GameObject(void * _pArg)
{
	CDummyTerrain* pInstance = new CDummyTerrain(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CTerrain", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDummyTerrain::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
