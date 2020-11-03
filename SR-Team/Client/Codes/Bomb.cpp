#include "stdafx.h"
#include "..\Headers\Bomb.h"

USING(Client)

CBomb::CBomb(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CBomb::CBomb(const CBomb& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CBomb::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBomb::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_vStartPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBomb::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (Movement(_fDeltaTime))
		return 0;

	return 0;
}

_int CBomb::LateUpdate_GameObject(_float _fDeltaTime)
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

HRESULT CBomb::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
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

HRESULT CBomb::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pGolemTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Golem", L"Com_Transform0");

	if (nullptr == pGolemTransform)
		return E_FAIL;

	_vec3 vPos = pGolemTransform->Get_Desc().vPosition;

	tTransformDesc.vPosition = { vPos.x , vPos.y, vPos.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.f , 0.5f , 1.f };
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

HRESULT CBomb::Movement(_float _fDeltaTime)
{
	if (!m_bOnece)
	{
		if (FAILED(Setting_Dir()))
			return E_FAIL;
	}

	if (FAILED(IsOnTerrain(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Dead_Bomb(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBomb::IsOnTerrain(_float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

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

	if (vPosition.y > vPos.y)
	{
		m_bFallDown = false;
	}

	if (m_bFallDown)
	{
		vPos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime;
		m_fJumpTime += _fDeltaTime;

		vPos += _vec3(m_vDir.x, 0.f, m_vDir.z) * (_fDeltaTime * 10.f);
		m_pTransformCom->Set_Position(_vec3(vPos.x, vPos.y, vPos.z));
	}
	else if (!m_bFallDown)
	{
		m_bStart = true;
		D3DXVECTOR3 vLastPos = m_pTransformCom->Get_Desc().vPosition;
		m_pTransformCom->Set_Position(_vec3(vLastPos.x, vPosition.y, vLastPos.z));
	}

	return S_OK;
}
CGameObject* CBomb::Clone_GameObject(void * _pArg)
{
	CBomb* pInstance = new CBomb(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CBomb", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBomb::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

CBomb * CBomb::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CBomb* pInstance = new CBomb(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CBomb", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CBomb::Setting_Dir()
{
	_vec3 vPos = m_pTransformCom->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

	m_vDir = m_vStartPos - vPos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_bFallDown = true;
	m_bOnece = true;

	return S_OK;
}

HRESULT CBomb::Dead_Bomb(_float _fDeltaTime)
{
	if (m_bStart)
	{
		m_fBombTime += _fDeltaTime;
		m_fDeadTime += _fDeltaTime;

		if (m_iTexCnt == 0 && m_fBombTime >= 0.6f)
		{
			m_iTexCnt = 1;
			m_fBombTime = 0.f;
		}
		else if (m_iTexCnt == 1 && m_fBombTime >= 0.6f)
		{
			m_iTexCnt = 0;
			m_fBombTime = 0.f;
		}
	}

	if (m_fDeadTime >= 3.f)
	{
		m_bStart = false;
		m_bDead = true;
	}

	return S_OK;
}
