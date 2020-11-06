#include "stdafx.h"
#include "..\Headers\ThunderBox.h"
#include "DamageInfo.h"

USING(Client)

CThunderBox::CThunderBox(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CThunderBox::CThunderBox(const CThunderBox& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CThunderBox::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CThunderBox::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CThunderBox::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return E_FAIL;

	m_fDeadTime += _fDeltaTime;

	if (m_fDeadTime > 4.f)
	{
		m_bDead = true;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CThunderBox::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CGameObject * CThunderBox::Clone_GameObject(void * _pArg)
{
	CThunderBox* pInstance = new CThunderBox(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create Clone CThunderBox", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CThunderBox::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
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

HRESULT CThunderBox::Take_Damage(const CComponent* _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;

	CGameObject* pOwner = ((CDamageInfo*)_pDamageComp)->Get_Desc().pOwner;//->Get_Component(L"Com_Transform0");
	CTransform* pTransform = (CTransform*)pOwner->Get_Component(L"Com_Transform0");
	_vec3 vBeaterPos = pTransform->Get_Desc().vPosition;

	if (FAILED(Spawn_ThunderStorm(L"Layer_ThunderStorm", vBeaterPos)))
		return E_FAIL;


	return S_OK;
}



CThunderBox * CThunderBox::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CThunderBox* pInstance = new CThunderBox(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CThunderBox", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThunderBox::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

HRESULT CThunderBox::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	_vec3 vDir = _vec3(m_tInstant.vDirection.x, 0.f, m_tInstant.vDirection.z);
	D3DXVec3Normalize(&vDir, &vDir);
	tTransformDesc.vPosition = m_tInstant.vPosition + (vDir * 4.f);
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 4.f , 0.5f , 4.f };
	//-------------------------------------------------------

	//-------------------------------------------------------
	// Collider Setting
	//-------------------------------------------------------
	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.5f * 4.f;
	//-------------------------------------------------------
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Stone", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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
		tDmgInfo.iMinAtt = pOwnerStatusComp->Get_Status().iMinAtt + m_pStatusCom->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = pOwnerStatusComp->Get_Status().iMaxAtt + m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalRate = pOwnerStatusComp->Get_Status().iCriticalRate + m_pStatusCom->Get_Status().iCriticalRate;
		tDmgInfo.iCriticalChance = pOwnerStatusComp->Get_Status().iCriticalChance + m_pStatusCom->Get_Status().iCriticalChance;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
		tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
	}

	tDmgInfo.eType = NONE;


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CThunderBox::Spawn_ThunderStorm(const wstring & LayerTag, _vec3 _vTargetPos)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_ThunderStorm", SCENE_STAGE0, LayerTag, &_vTargetPos)))
		return E_FAIL;

	return S_OK;
}
