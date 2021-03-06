#include "stdafx.h"
#include "..\Headers\Stump_Impact.h"
#include "DamageInfo.h"
#include "..\Headers\Wolf_Impact.h"

USING(Client)

CStump_Impact::CStump_Impact(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CStump_Impact::CStump_Impact(const CStump_Impact& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CStump_Impact::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CStump_Impact::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CStump_Impact::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime >= 1.8f)
		m_bDead = true;

	return GAMEOBJECT::NOEVENT;
}

_int CStump_Impact::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CStump_Impact::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	_vec3 vGolemLook = _vec3(m_tInstant.vDirection.x, 0.f, m_tInstant.vDirection.z);
	D3DXVec3Normalize(&vGolemLook, &vGolemLook);
	_vec3 vPosition = { m_tInstant.vPosition.x , 0.f, m_tInstant.vPosition.z };
	vPosition += vGolemLook * 3.f;
	tTransformDesc.vPosition = vPosition;
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 10.f , 5.f , 10.f };

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.5f * 10.f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	CStatus::STAT tStat;
	tStat.iCriticalChance = 1;	tStat.iCriticalRate = 1;
	tStat.iMinAtt = 20;			tStat.iMaxAtt =20;
	tStat.fAttRate = 1.f;		tStat.fDefRate = 1.f;


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CDamageInfo::DAMAGE_DESC tDmgInfo;
	tDmgInfo.pOwner = m_tInstant.pAttacker;
	if (m_tInstant.pStatusComp)
	{
		CStatus* pOnwerStatusComp = (CStatus*)m_tInstant.pStatusComp;
		tDmgInfo.iMinAtt = pOnwerStatusComp->Get_Status().iMinAtt + m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iMaxAtt = pOnwerStatusComp->Get_Status().iMaxAtt + m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalChance = pOnwerStatusComp->Get_Status().iCriticalChance + m_pStatusCom->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = pOnwerStatusComp->Get_Status().iCriticalRate + m_pStatusCom->Get_Status().iCriticalRate;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
	}

	tDmgInfo.eType = NONE;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}

CGameObject * CStump_Impact::Clone_GameObject(void * _pArg)
{
	CStump_Impact* pInstance = new CStump_Impact(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create CStump_Impact Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CStump_Impact::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return E_FAIL;

	return S_OK;
}

void CStump_Impact::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CStump_Impact * CStump_Impact::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CStump_Impact* pInstance = new CStump_Impact(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CStump_Impact", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}