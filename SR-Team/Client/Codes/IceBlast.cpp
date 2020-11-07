#include "stdafx.h"
#include "Status.h"
#include "DamageInfo.h"
#include "..\Headers\IceBlast.h"

USING(Client)

CIceBlast::CIceBlast(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CIceBlast::CIceBlast(const CIceBlast & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CIceBlast::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CIceBlast::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tImpact = *(INSTANTIMPACT*)_pArg;
		m_fInitDelay *= m_tImpact.vOption.x;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;


	return S_OK;
}

_int CIceBlast::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	// falldown
	_vec3 vAddPos = _vec3(0.f, -1.f, 0.f) * m_pTransformComp->Get_Desc().fSpeedPerSecond * _fDeltaTime;
	m_pTransformComp->Set_Position(m_pTransformComp->Get_Desc().vPosition + vAddPos);
	if (m_pTransformComp->Get_Desc().vPosition.y < m_tImpact.vPosition.y)
		m_bDead = true;

	m_pTransformComp->Update_Transform();

	m_pColliderCom->Update_Collider(m_pTransformComp->Get_Desc().vPosition);

	return GAMEOBJECT::NOEVENT;
}

_int CIceBlast::LateUpdate_GameObject(_float _fDeltaTime)
{
	// init delay counting
	Update_InitDelay(_fDeltaTime);
	Update_DeadDelay(_fDeltaTime);
	
	if (!m_bActive)
		GAMEOBJECT::NOEVENT;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CIceBlast::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");

	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferComp->Set_Transform(&m_pTransformComp->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureComp->SetTexture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferComp->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CIceBlast::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = { m_tImpact.vPosition.x, m_tImpact.vPosition.y + 14.f ,m_tImpact.vPosition.z};

	_float fScale = m_tImpact.vOption.x;
	tTransformDesc.vScale = { 1.f, 1.f, 1.f };
	tTransformDesc.vRotate = { D3DXToRadian(_float(rand()%90)), D3DXToRadian(_float(rand() % 90)), D3DXToRadian(_float(rand() % 90)) };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	CSphereCollider::COLLIDER_DESC tCollDesc;
	tCollDesc.vPosition = tTransformDesc.vPosition;
	tCollDesc.fRadius = 0.5f;

	CStatus::STAT tStat;
	tStat.iMinAtt = 10; tStat.iMaxAtt = 10;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferComp)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_IceBlast", L"Com_Texture", (CComponent**)&m_pTextureComp)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformComp, &tTransformDesc)))
		return E_FAIL;

	// For.Collider
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tCollDesc)))
		return E_FAIL;

	// For.Status
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusComp, &tStat)))
		return E_FAIL;

	CStatus* pOwnerStatusComp = (CStatus*)m_tImpact.pStatusComp;
	CDamageInfo::DAMAGE_DESC tDmgInfo;

	if (pOwnerStatusComp)
	{
		tDmgInfo.pOwner = m_tImpact.pAttacker;
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

void CIceBlast::Update_InitDelay(_float _fDeltaTime)
{
	if (!IsActive())
	{
		m_fInitTimer += _fDeltaTime;
		if (m_fInitTimer >= m_fInitDelay)
			Set_Active();
	}
}

void CIceBlast::Update_DeadDelay(_float _fDeltaTime)
{
	if (IsActive() && !m_bDead)
	{
		m_fDeadTimer += _fDeltaTime;
		if (m_fDeadTimer >= m_fDeadDelay)
			m_bDead = true;
	}
}

CGameObject * CIceBlast::Clone_GameObject(void * _pArg)
{
	CIceBlast* pInstance = new CIceBlast(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create EnergyBolt Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBlast::Free()
{
	Safe_Release(m_pVIBufferComp);
	Safe_Release(m_pTransformComp);
	Safe_Release(m_pTextureComp);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusComp);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CIceBlast * CIceBlast::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CIceBlast* pInstance = new CIceBlast(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CIceBlast", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
