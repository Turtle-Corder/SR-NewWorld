#include "stdafx.h"
#include "..\Headers\EnergyBolt.h"
#include "Status.h"
#include "DamageInfo.h"

USING(Client)

CEnergyBolt::CEnergyBolt(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CEnergyBolt::CEnergyBolt(const CEnergyBolt& _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CEnergyBolt::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CEnergyBolt::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CEnergyBolt::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

_int CEnergyBolt::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CEnergyBolt::Render_NoneAlpha()
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

HRESULT CEnergyBolt::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = m_tInstant.vPosition;
	tTransformDesc.vScale = { 0.33f, 0.33f, 0.33f };
	tTransformDesc.vRotate = { D3DXToRadian(45.f), D3DXToRadian(-90.f), 0.f };
	tTransformDesc.fSpeedPerSecond = 4.5f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(160.f);

	CSphereCollider::COLLIDER_DESC tCollDesc;
	tCollDesc.vPosition = tTransformDesc.vPosition;
	tCollDesc.fRadius = 0.18f;

	CStatus::STAT tStat;
	tStat.iCriticalHit = 0; tStat.iCriticalRate = 0;
	tStat.iMinAtt = 10; tStat.iMaxAtt = 10;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_EnergyBolt", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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
	tDmgInfo.eType = NONE;

	//For.DamageInfo
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnergyBolt::Movement(_float _fDeltaTime)
{
	if (FAILED(Move(_fDeltaTime)))
		return E_FAIL;

	m_fTestDeadTime += _fDeltaTime;

	if (m_fTestDeadTime >= 3.f)
		m_bDead = true;

	return S_OK;
}

HRESULT CEnergyBolt::Move(_float _fDeltaTime)
{
	_vec3 vAddPos = m_tInstant.vDirection * (_fDeltaTime * m_pTransformCom->Get_Desc().fSpeedPerSecond);
	m_pTransformCom->Set_Position(m_pTransformCom->Get_Desc().vPosition + vAddPos);

//	m_pTransformCom->Turn(CTransform::AXIS_X, _fDeltaTime * m_pTransformCom->Get_Desc().fRotatePerSecond);
	m_pTransformCom->Turn(CTransform::AXIS_Y, _fDeltaTime * m_pTransformCom->Get_Desc().fRotatePerSecond * 1.2f);
	//m_pTransformCom->Turn(CTransform::AXIS_Z, _fDeltaTime * m_pTransformCom->Get_Desc().fRotatePerSecond);

	//m_pTransformCom->Update_Transform();
	return S_OK;
}

CGameObject * CEnergyBolt::Clone_GameObject(void * _pArg)
{
	CEnergyBolt* pInstance = new CEnergyBolt(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create EnergyBolt Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnergyBolt::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusComp);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CEnergyBolt * CEnergyBolt::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if(nullptr == _pDevice)
		return nullptr;

	CEnergyBolt* pInstance = new CEnergyBolt(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create EnergyBolt", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CEnergyBolt::Take_Damage(const CComponent * _pDamageComp)
{
	m_bDead = true;

	return S_OK;
}
