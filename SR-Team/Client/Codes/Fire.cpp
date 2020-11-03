#include "stdafx.h"
#include "..\Headers\Fire.h"

USING(Client)

CFire::CFire(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CFire::CFire(const CFire& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CFire::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CFire::Setup_GameObject(void * _pArg)
{
	// 스케일말고 포지션으로 위로올리기
	if (_pArg)
		m_vStartPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CFire::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (Movement(_fDeltaTime))
		return 0;

	return 0;
}

_int CFire::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CFire::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(m_iTexCnt)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFire::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vPosition = { m_vStartPos.x , m_vStartPos.y, m_vStartPos.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.f , 1.f , 1.f };
	//--------------------------------------------------
	// VIBuffer Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	//--------------------------------------------------
	// Transform Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	//--------------------------------------------------
	// Texture Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Bomb", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CFire::Movement(_float _fDeltaTime)
{
	if (FAILED(IsOnTerrain(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Dead_Fire(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFire::IsOnTerrain(_float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

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
CGameObject* CFire::Clone_GameObject(void * _pArg)
{
	CFire* pInstance = new CFire(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CFire", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFire::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

CFire * CFire::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CFire* pInstance = new CFire(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CBomb", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CFire::Dead_Fire(_float _fDeltaTime)
{
	m_fDeadTime += _fDeltaTime + _fDeltaTime;

	if (m_fDeadTime >= 10.f)
		m_bDead = true;
	else
		m_pTransformCom->Set_Scale(_vec3(1.f, m_fDeadTime, 1.f));

	return S_OK;
}
