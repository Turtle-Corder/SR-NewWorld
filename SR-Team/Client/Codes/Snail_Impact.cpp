#include "stdafx.h"
#include "..\Headers\Snail_Impact.h"
#include "DamageInfo.h"

USING(Client)

CSnail_Impact::CSnail_Impact(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CSnail_Impact::CSnail_Impact(const CSnail_Impact & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CSnail_Impact::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSnail_Impact::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CSnail_Impact::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime >= 1.f)
		m_bDead = true;

	m_pTransformCom->Update_Transform();

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CSnail_Impact::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CSnail_Impact::Add_Component()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;
	_vec3 vGolemLook = _vec3(m_tInstant.vDirection.x, 0.f, m_tInstant.vDirection.z);
	D3DXVec3Normalize(&vGolemLook, &vGolemLook);
	_vec3 vPosition = { m_tInstant.vPosition.x , 0.f, m_tInstant.vPosition.z };
	vPosition += vGolemLook * 1.f;
	tTransformDesc.vPosition = vPosition;
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.f , 1.f , 1.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.7f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	CStatus::STAT tStat;
	tStat.iCriticalChance = 0;	tStat.iCriticalRate = 0;
	tStat.iMinAtt = 20;			tStat.iMaxAtt = 20;

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

CGameObject * CSnail_Impact::Clone_GameObject(void * _pArg)
{
	CSnail_Impact* pInstance = new CSnail_Impact(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create Snail_Impact Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CSnail_Impact::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return E_FAIL;

	return S_OK;
}

void CSnail_Impact::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CSnail_Impact * CSnail_Impact::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSnail_Impact* pInstance = new CSnail_Impact(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create Snail_Impact", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
