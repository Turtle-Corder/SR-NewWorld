#include "stdafx.h"
#include "Status.h"
#include "..\Headers\Snow.h"
#include "DamageInfo.h"

USING(Client)

CSnow::CSnow(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CSnow::CSnow(const CSnow & other)
	: CGameObject(other)
{
}

HRESULT CSnow::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSnow::Setup_GameObject(void * pArg)
{
	if (pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CSnow::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

int CSnow::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;


	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CSnow::Render_NoneAlpha()
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

HRESULT CSnow::Add_Component()
{
	//-------------------------------------------------------
	// Transform Setting
	//-------------------------------------------------------
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = m_tInstant.vPosition;
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 0.5f , 0.5f , 0.5f };
	//-------------------------------------------------------

	//-------------------------------------------------------
	// Collider Setting
	//-------------------------------------------------------
	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.7f;
	//-------------------------------------------------------


	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), L"Component_Texture_Snow", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	// For.Com_Transform

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	// TODO : m_pStatusComp 셋팅
	CStatus::STAT tStat;
	tStat.iCriticalRate = 0;	tStat.iCriticalHit = 0;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 10;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusComp, &tStat)))
		return E_FAIL;

	CStatus* pOwnerStatusComp = (CStatus*)m_tInstant.pStatusComp;	
	CDamageInfo::DAMAGE_DESC tDmgInfo;
	if (pOwnerStatusComp)
	{
		tDmgInfo.pOwner = m_tInstant.pAttacker;

		// ex) yeti의 공격력 + 눈덩이 자체의 공격력 -> player의 기본 공격력 + 스태프의 공격력
		tDmgInfo.iMinAtt = pOwnerStatusComp->Get_Status().iMinAtt + m_pStatusComp->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = pOwnerStatusComp->Get_Status().iMaxAtt + m_pStatusComp->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalHit = pOwnerStatusComp->Get_Status().iCriticalHit + m_pStatusComp->Get_Status().iCriticalHit;
		tDmgInfo.iCriticalRate = pOwnerStatusComp->Get_Status().iCriticalRate + m_pStatusComp->Get_Status().iCriticalRate;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusComp->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = m_pStatusComp->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalHit = m_pStatusComp->Get_Status().iCriticalHit;
		tDmgInfo.iCriticalRate = m_pStatusComp->Get_Status().iCriticalRate;
	}

	tDmgInfo.eType = ICE;


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	if (FAILED(Setting_Dir()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSnow::Movement(float _fDeltaTime)
{
	if (FAILED(Throwing_Snow(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return E_FAIL;

	m_fDeadTime += _fDeltaTime;

	if (m_fDeadTime >= 10.f)
		m_bDead = true;


	return S_OK;
}

HRESULT CSnow::Setting_Dir()
{
	_vec3 vPos = m_pTransformCom->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform1");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

	m_vDir = vPlayerPos - vPos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_bFallDown = true;

	return S_OK;
}

CSnow * CSnow::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSnow* pInstance = new CSnow(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSnow", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSnow::Clone_GameObject(void * pArg)
{
	CSnow* pInstance = new CSnow(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSnow", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnow::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusComp);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

HRESULT CSnow::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;

	m_bDead = true;

	return S_OK;
}

HRESULT CSnow::Throwing_Snow(float _fDeltaTime)
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

	if (m_bFallDown)
	{
		vPos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime;
		m_fJumpTime += _fDeltaTime;

		vPos += _vec3(m_vDir.x * 2.f, 0.f, m_vDir.z * 2.f) * (_fDeltaTime * 8.f);
	}

	if (vPosition.y > vPos.y)
	{
		m_bFallDown = false;
		m_bDead = true;
	}

	m_pTransformCom->Set_Position(_vec3(vPos.x, vPos.y, vPos.z));

	return S_OK;
}
