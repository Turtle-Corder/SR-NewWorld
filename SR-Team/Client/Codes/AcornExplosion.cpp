#include "stdafx.h"
#include "..\Headers\AcornExplosion.h"

USING(Client)

CAcornExplosion::CAcornExplosion(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CAcornExplosion::CAcornExplosion(const CAcornExplosion& other)
	: CGameObject(other)

{
}

HRESULT CAcornExplosion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CAcornExplosion::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CAcornExplosion::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	m_fDeadTime += _fDeltaTime;

	if (m_fDeadTime >= 3.f)
		m_bDead = true;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

int CAcornExplosion::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_ONLYALPHA, this)))
		return  GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

HRESULT CAcornExplosion::Render_OnlyAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
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

HRESULT CAcornExplosion::Add_Component()
{
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_FOREST, L"Component_Texture_AcornExplosion", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vPosition = m_tInstant.vPosition;
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 0.5f , 0.5f , 0.5f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	m_vDir.x = (_float)(rand() % 5 - 10);
	m_vDir.y = 0.f;
	m_vDir.z = (_float)(rand() % 5 - 10);

	if (m_vDir.x == 0 && m_vDir.z == 0)
	{
		m_vDir.x = (_float)(rand() % 50 - 100);
		m_vDir.z = (_float)(rand() % 50 - 100);
	}
	return S_OK;
}

HRESULT CAcornExplosion::Movement(float _fDeltaTime)
{
	_vec3 vMyPos = m_pTransformCom->Get_Desc().vPosition;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	vMyPos += m_vDir * (_fDeltaTime * 5.f);
	m_pTransformCom->Set_Position(vMyPos);

	return S_OK;
}


HRESULT CAcornExplosion::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPosition = m_pTransformCom->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom->Set_Position(vPosition);
	}

	return S_OK;
}

CAcornExplosion* CAcornExplosion::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CAcornExplosion* pInstance = new CAcornExplosion(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CPlaneSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAcornExplosion::Clone_GameObject(void * pArg)
{
	CAcornExplosion* pInstance = new CAcornExplosion(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CPlaneSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAcornExplosion::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CAcornExplosion::Take_Damage(const CComponent * _pDamageComp)
{

	return S_OK;
}
