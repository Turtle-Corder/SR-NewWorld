#include "stdafx.h"
#include "MainCamera.h"
#include "DamageInfo.h"
#include "Player.h"
#include "..\Headers\EventTrigger.h"


USING(Client)

CEventTrigger::CEventTrigger(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CEventTrigger::CEventTrigger(const CEventTrigger & other)
	: CGameObject(other)
{
}

HRESULT CEventTrigger::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CEventTrigger::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInfo = *(EVENT_INFO*)_pArg;
	else
		ZeroMemory(&m_tInfo, sizeof(EVENT_INFO));

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CEventTrigger::Update_GameObject(float DeltaTime)
{
	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

int CEventTrigger::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return 0;
}

HRESULT CEventTrigger::Render_NoneAlpha()
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

HRESULT CEventTrigger::Take_Damage(const CComponent * _pDamageComp)
{
	CDamageInfo* pDamageInfo = (CDamageInfo*)_pDamageComp;
	if (nullptr == pDamageInfo)
		return E_FAIL;

	CPlayer* pPlayer = (CPlayer*)pDamageInfo->Get_Desc().pOwner;
	if (pPlayer || pPlayer->IsInteraction())
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

//		pManagement->Get_
	}


	return S_OK;
}

HRESULT CEventTrigger::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORM_DESC));
	tTransformDesc.vPosition = m_tInfo.vSpawnPos;


	//VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Trigger", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	//--------------------------------------------------
	// Collider
	//--------------------------------------------------
	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 1.f;

	//Collider
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEventTrigger::Floating(_float _fDeltaTime)
{
	m_fTimeFlow += _fDeltaTime;

	if (m_fTimeFlow >= D3DX_PI)
		m_fTimeFlow = 0;

	_vec3 Temp = m_pTransformCom->Get_Desc().vPosition;

	Temp.y = sinf(m_fTimeFlow);

	m_pTransformCom->Set_Position(Temp);

	return S_OK;
}

CEventTrigger * CEventTrigger::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CEventTrigger* pInstance = new CEventTrigger(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(_T("Failed To Create CTree"), LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEventTrigger::Clone_GameObject(void * pArg)
{
	CEventTrigger* pInstance = new CEventTrigger(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(_T("Failed To Clone CTree"), LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEventTrigger::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
