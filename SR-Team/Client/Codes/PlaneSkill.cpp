#include "stdafx.h"
#include "..\Headers\PlaneSkill.h"

USING(Client)

CPlaneSkill::CPlaneSkill(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CPlaneSkill::CPlaneSkill(const CPlaneSkill & other)
	: CGameObject(other)

{
}

HRESULT CPlaneSkill::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPlaneSkill::Setup_GameObject(void * pArg)
{
	if (pArg)
		m_vGoalPos = *(_vec3*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CPlaneSkill::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

int CPlaneSkill::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_ONLYALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CPlaneSkill::Render_OnlyAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
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

	return S_OK;
}

HRESULT CPlaneSkill::Add_Component()
{
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_SpellJin", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;

	// TerrainY 로 부터 높이 구해서 20.f

	tTransformDesc.vPosition = { m_vGoalPos.x, m_vGoalPos.y, m_vGoalPos.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 5.f , 5.f , 5.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	m_pTransformCom->Turn(CTransform::AXIS_X, 45);

	return S_OK;
}

HRESULT CPlaneSkill::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	return S_OK;
}


HRESULT CPlaneSkill::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPosition = m_pTransformCom->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom->Set_Position(vPosition);
	}

	return S_OK;
}

CPlaneSkill * CPlaneSkill::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CPlaneSkill* pInstance = new CPlaneSkill(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CPlaneSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlaneSkill::Clone_GameObject(void * pArg)
{
	CPlaneSkill* pInstance = new CPlaneSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CPlaneSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlaneSkill::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
