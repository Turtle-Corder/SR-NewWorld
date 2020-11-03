#include "stdafx.h"
#include "MainCamera.h"
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
	vector<void*> GetVector;
	_vec3 vPos;

	if (_pArg)
	{
		GetVector = (*(vector<void*>*)(_pArg));
		vPos = (*(_vec3*)GetVector[0]);
		m_eEventName = (*(EVENT_NUM*)GetVector[1]);

		if (m_eEventName >= EVENT_NUM::EVENT_END)
			return E_FAIL;
	}

	if (FAILED(Add_Component(vPos)))
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

HRESULT CEventTrigger::Add_Component()
{

	return S_OK;
}

HRESULT CEventTrigger::Add_Component(_vec3 _vPos)
{
	_vec3 vPositon_Body = _vPos + _vec3(0.f, 0.5f, 0.f);
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = vPositon_Body;



	//--------------------------------------------------
	// Collider
	//--------------------------------------------------
	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = m_pTransformCom->Get_Desc().vPosition;
	tColDesc.fRadius = 1.f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	//VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Translucent_Cube", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CEventTrigger::Collision_Check()
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return E_FAIL;

	switch (m_eEventName)
	{
	case Client::CEventTrigger::PORTAL:


		//대충 이런식으로 여기에 씬이동시키는 함수를 소환
		break;


	case Client::CEventTrigger::TWODIMENVIEW:


		CMainCamera* pCamera = (CMainCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
		if (nullptr == pCamera)
			return E_FAIL;

		pCamera->Set_Camera_Mode(CMainCamera::CAMERA_VIEWMODE::CAMERA_2D_X);

		break;

	case Client::CEventTrigger::THREEDIMENVIEW:

		CMainCamera* pCamera = (CMainCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
		if (nullptr == pCamera)
			return E_FAIL;

		pCamera->Set_Camera_Mode(CMainCamera::CAMERA_VIEWMODE::CAMERA_3D);

		break;

	default:
		break;
	}

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
	

	CGameObject::Free();
}
