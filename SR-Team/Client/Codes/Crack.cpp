#include "stdafx.h"
#include "..\Headers\Crack.h"

USING(Client)

CCrack::CCrack(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CCrack::CCrack(const CCrack& other)
	: CGameObject(other)

{
}

HRESULT CCrack::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCrack::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_vGoalPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CCrack::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

int CCrack::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_ONLYALPHA, this)))
		return  GAMEOBJECT::WARN;

	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime >= 1.f)
		m_bDead = true;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CCrack::Render_OnlyAlpha()
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

HRESULT CCrack::Add_Component()
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), L"Component_Texture_Crack", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

HRESULT CCrack::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	return S_OK;
}


HRESULT CCrack::IsOnTerrain()
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

CCrack * CCrack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CCrack* pInstance = new CCrack(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CPlaneSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCrack::Clone_GameObject(void * pArg)
{
	CCrack* pInstance = new CCrack(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CPlaneSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrack::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
