#include "stdafx.h"
#include "..\Headers\IceSpear.h"
#include "Status.h"
#include "DamageInfo.h"

USING(Client)

CIceSpear::CIceSpear(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CIceSpear::CIceSpear(const CIceSpear& other)
	: CGameObject(other)
{
}

HRESULT CIceSpear::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CIceSpear::Setup_GameObject(void* _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vMoveDir = m_tInstant.vDirection;
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);


	return S_OK;
}

int CIceSpear::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	
	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

int CIceSpear::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime >= 3.f)
		m_bDead = true;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CIceSpear::Render_NoneAlpha()
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

HRESULT CIceSpear::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	//시작할위치
	tTransformDesc.vPosition = { _vec3(m_tInstant.vPosition.x + 0.05f , m_tInstant.vPosition.y , m_tInstant.vPosition.z - 0.05f) };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(30.f);
	tTransformDesc.vScale = { 0.5f, 3.f, 0.5f };

	CSphereCollider::COLLIDER_DESC tCollDesc;
	tCollDesc.vPosition = tTransformDesc.vPosition;
	tCollDesc.fRadius = 0.5f;

	CStatus::STAT tStat;
	tStat.iCriticalChance = 0; tStat.iCriticalRate = 0;
	tStat.iMinAtt = 10; tStat.iMaxAtt = 10;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Pyramid", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_IceSpear", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	// For.Collider
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tCollDesc)))
		return E_FAIL;

	// For.Status
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusComp, &tStat)))
		return E_FAIL;

	CStatus* pOwnerStatusComp = (CStatus*)m_tInstant.pStatusComp;
	CDamageInfo::DAMAGE_DESC tDmgInfo;

	if (pOwnerStatusComp)
	{
		tDmgInfo.pOwner = m_tInstant.pAttacker;
		tDmgInfo.iMinAtt = pOwnerStatusComp->Get_Status().iMinAtt + m_pStatusComp->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = pOwnerStatusComp->Get_Status().iMaxAtt + m_pStatusComp->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalChance = pOwnerStatusComp->Get_Status().iCriticalChance + m_pStatusComp->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = pOwnerStatusComp->Get_Status().iCriticalRate + m_pStatusComp->Get_Status().iCriticalRate;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusComp->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = m_pStatusComp->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalChance = m_pStatusComp->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = m_pStatusComp->Get_Status().iCriticalRate;
	}

	tDmgInfo.eType = ICE;

	//For.DamageInfo
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CIceSpear::Movement(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vAddPos = m_vMoveDir * m_pTransformCom->Get_Desc().fSpeedPerSecond * _fDeltaTime;
	m_pTransformCom->Set_Position(m_pTransformCom->Get_Desc().vPosition + vAddPos);

	if (m_pTransformCom->Get_Desc().vPosition.y < m_tInstant.vOption.y)
		m_bDead = true;
	_matrix matRotation;

	D3DXMatrixRotationAxis(&matRotation, &m_vMoveDir, D3DX_PI / 2.f);

	if (FAILED(m_pTransformCom->Update_Transform_Rotate(&matRotation)))
		return GAMEOBJECT::WARN;



	return S_OK;
}

CIceSpear * CIceSpear::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CIceSpear* pInstance = new CIceSpear(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMeteor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CIceSpear::Clone_GameObject(void * pArg)
{
	CIceSpear* pInstance = new CIceSpear(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMeteor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceSpear::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusComp);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

HRESULT CIceSpear::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;



	return S_OK;
}
