#include "stdafx.h"
#include "..\Headers\Thunder.h"
#include "DamageInfo.h"

USING(Client)

CThunderStorm::CThunderStorm(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
	m_bDead = false;
	m_tInstant = {};
	m_fMoveTime = 0.f;
	m_fDeadTime = 1.f;
	m_fDeadTimer = 0.f;
}

CThunderStorm::CThunderStorm(const CThunderStorm & _rOther)
	: CGameObject(_rOther)
{
		m_bDead = false;
		m_tInstant = {};
		m_fMoveTime = 0.f;
		m_fDeadTime = 1.f;
		m_fDeadTimer = 0.f;
}

HRESULT CThunderStorm::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CThunderStorm::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CThunderStorm::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return E_FAIL;

	Check_Dead(_fDeltaTime);

	return GAMEOBJECT::NOEVENT;
}

_int CThunderStorm::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CGameObject * CThunderStorm::Clone_GameObject(void * _pArg)
{
	CThunderStorm* pInstance = new CThunderStorm(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create Clone CThunderStorm", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CThunderStorm::Render_NoneAlpha()
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

HRESULT CThunderStorm::Render_BlendAlpha()
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

HRESULT CThunderStorm::Take_Damage(const CComponent* _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;


	return S_OK;
}



CThunderStorm * CThunderStorm::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CThunderStorm* pInstance = new CThunderStorm(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CThunderStorm", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThunderStorm::Check_Dead(_float _DeltaTime)
{
	m_fDeadTimer += _DeltaTime;
	if (m_fDeadTimer > m_fDeadTime)
		m_bDead = true;
}


void CThunderStorm::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

HRESULT CThunderStorm::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = m_tInstant.vPosition;
	tTransformDesc.vPosition.y += 2.f;
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(1.f);
	tTransformDesc.vScale = { 10.f , 10.f , 10.f };
	//-------------------------------------------------------

	//-------------------------------------------------------
	// Collider Setting
	//-------------------------------------------------------
	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 7.f;
	//-------------------------------------------------------
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Thunder", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Transform

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	// TODO : m_pStatusComp 셋팅
	CStatus::STAT tStat;
	tStat.iCriticalChance = 0;	tStat.iCriticalRate = 0;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 10;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CStatus* pOwnerStatusComp = (CStatus*)m_tInstant.pStatusComp;
	CDamageInfo::DAMAGE_DESC tDmgInfo;
	if (pOwnerStatusComp)
	{
		tDmgInfo.pOwner = m_tInstant.pAttacker;

		// ex) yeti의 공격력 + 눈덩이 자체의 공격력 -> player의 기본 공격력 + 스태프의 공격력
		tDmgInfo.iMinAtt = pOwnerStatusComp->Get_Status().iMinAtt * pOwnerStatusComp->Get_Status().fAttRate + m_pStatusCom->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = pOwnerStatusComp->Get_Status().iMaxAtt * pOwnerStatusComp->Get_Status().fAttRate + m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalChance = pOwnerStatusComp->Get_Status().iCriticalChance + m_pStatusCom->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = pOwnerStatusComp->Get_Status().iCriticalRate + m_pStatusCom->Get_Status().iCriticalRate;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
		tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
	}

	tDmgInfo.eType = NONE;


	m_pTransformCom->Turn(CTransform::AXIS_X, 90.f);
	m_pTransformCom->Update_Transform();

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	//m_iMaxCnt = m_pTextureCom->Get_TextureCount();
	//m_pTextureCom->SetFrameRange(0, m_iMaxCnt - 1);

	return S_OK;
}
