#include "stdafx.h"
#include "..\Headers\Golem_Impact.h"
#include "DamageInfo.h"

USING(Client)

CGolem_Impact::CGolem_Impact(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CGolem_Impact::CGolem_Impact(const CGolem_Impact& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CGolem_Impact::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CGolem_Impact::Setup_GameObject(void* _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CGolem_Impact::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	m_pTransformCom->Update_Transform();

	m_pTransformCom->Set_WorldMatrix(m_matGolemRot * m_pTransformCom->Get_Desc().matWorld);

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime >= 1.f)
		m_bDead = true;

	return GAMEOBJECT::NOEVENT;
}

_int CGolem_Impact::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CGolem_Impact::Render_NoneAlpha()
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

HRESULT CGolem_Impact::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	_vec3 vGolemLook = _vec3(m_tInstant.vDirection.x, 0.f, m_tInstant.vDirection.z);
	D3DXVec3Normalize(&vGolemLook, &vGolemLook);
	_vec3 vPosition = { m_tInstant.vPosition.x , 0.f, m_tInstant.vPosition.z };
	vPosition -= vGolemLook * 3.f;

	tTransformDesc.vPosition = vPosition;
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 3.f , 3.f , 3.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom))) //积己 肮荐
		return E_FAIL;


	if (FAILED(CGameObject::Add_Component(SCENE_VOLCANIC, L"Component_Texture_Wolf_Face", L"Com_Texture", (CComponent**)&m_pTextureCom))) ////积己 肮荐
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.5f * 3.f;

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
		tDmgInfo.iCriticalRate = pOnwerStatusComp->Get_Status().iCriticalChance + m_pStatusCom->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = pOnwerStatusComp->Get_Status().iCriticalRate + m_pStatusCom->Get_Status().iCriticalRate;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
	}

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pGolemTransform = (CTransform*)pManagement->Get_Component(SCENE_VOLCANIC, L"Layer_Golem", L"Com_Transform0");
	if (nullptr == pGolemTransform)
		return E_FAIL;

	D3DXMatrixIdentity(&m_matGolemRot);
	_matrix GolemWorld = pGolemTransform->Get_Desc().matWorld;

	m_matGolemRot._11 = GolemWorld._11;
	m_matGolemRot._13 = GolemWorld._13;
	m_matGolemRot._31 = GolemWorld._31;
	m_matGolemRot._33 = GolemWorld._33;


	INSTANTIMPACT tImpact = {};

	if (m_tInstant.fOption != 0.f)
	{
		for (_uint j = 0; j < 40; j++)
		{
				_vec3 RandomPostionSelect = { (_float)(rand() % 30 - 15), 18.f + (_float)(rand() % 4 - 2) ,(_float)(rand() % 30 - 15) };

				tImpact.vPosition = m_pTransformCom->Get_Desc().vPosition;
				tImpact.vDirection = RandomPostionSelect;
				tImpact.vOption = RandomPostionSelect + m_pTransformCom->Get_Desc().vPosition;

				if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_MeteorPiece", pManagement->Get_CurrentSceneID(), L"Layer_Effect", &tImpact)))
				{
					PRINT_LOG(L"Failed To Spawn MeteorPiece", LOG::DEBUG);
					return false;
				}
		}
	}

	return S_OK;
}

CGameObject * CGolem_Impact::Clone_GameObject(void * _pArg)
{
	CGolem_Impact* pInstance = new CGolem_Impact(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create InstantImpact Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_Impact::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CGolem_Impact * CGolem_Impact::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CGolem_Impact* pInstance = new CGolem_Impact(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create InstantImpact", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CGolem_Impact::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return E_FAIL;

	m_bDead = true;

	return S_OK;
}
