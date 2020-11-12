#include "stdafx.h"
#include "..\Headers\Slime_Impact.h"
#include "DamageInfo.h"
USING(Client)

CSlime_Impact::CSlime_Impact(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CSlime_Impact::CSlime_Impact(const CSlime_Impact & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CSlime_Impact::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSlime_Impact::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CSlime_Impact::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CSlime_Impact::LateUpdate_GameObject(_float _fDeltaTime)
{
	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime >= 1.f)
		m_bDead = true;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CSlime_Impact::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = { m_tInstant.vPosition };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 0.5f , 0.5f , 0.5f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.7f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	CStatus::STAT tStat;
	tStat.iCriticalChance = 0;	tStat.iCriticalRate = 0;
	tStat.iMinAtt = 0;			tStat.iMaxAtt = 0;
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

CGameObject * CSlime_Impact::Clone_GameObject(void * _pArg)
{
	CSlime_Impact* pInstance = new CSlime_Impact(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create Slime_Impact Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CSlime_Impact::Take_Damage(const CComponent * _pDamageComp)
{
	if(!_pDamageComp)
	return E_FAIL;

	return S_OK;
}

void CSlime_Impact::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CSlime_Impact * CSlime_Impact::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSlime_Impact* pInstance = new CSlime_Impact(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create Slime_Impact", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
