#include "stdafx.h"
#include "..\Headers\Flower.h"

USING(Client)

CFlower::CFlower(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	for (int i = 0; i < FLOWER_END; i++)
	{
		m_pVIBufferCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}
}

CFlower::CFlower(const CFlower & other)
	: CGameObject(other)
{
}

HRESULT CFlower::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CFlower::Setup_GameObject(void * _pArg)
{
	vector<void*> GetVector;
	_vec3 vPos;


	if (_pArg)
	{
		GetVector = (*(vector<void*>*)(_pArg));
		vPos = (*(_vec3*)GetVector[0]);
	}

	if (FAILED(Add_Component(vPos)))
		return E_FAIL;

	return S_OK;
}

int CFlower::Update_GameObject(float DeltaTime)
{
	for (_uint iCnt = 0; iCnt < FLOWER_END; iCnt++)
	{
		if (FAILED(m_pTransformCom[iCnt]->Update_Transform()))
			return GAMEOBJECT::WARN;
	}


	return GAMEOBJECT::NOEVENT;
}

int CFlower::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return 0;
}

HRESULT CFlower::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int i = 0; i < BODY_ID::FLOWER_END; i++)
	{
		if (FAILED(m_pVIBufferCom[i]->Set_Transform(&m_pTransformCom[i]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[i]->SetTexture(20)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[i]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CFlower::Add_Component()
{ 
	CTransform::TRANSFORM_DESC tTransformDesc[FLOWER_END];

	return S_OK;
}

HRESULT CFlower::Add_Component(_vec3 _vPos)
{
	//TransFormDesc Setting
	CTransform::TRANSFORM_DESC tTransformDesc[FLOWER_END];

	for (_uint iFlowerPetal = 0; iFlowerPetal < FLOWER_END; iFlowerPetal++)
	{
		tTransformDesc[iFlowerPetal].vPosition = _vPos;
	}

	//For.Com_VIBuffer

	for (_uint iFlowerPetal = 0; iFlowerPetal < FLOWER_END; iFlowerPetal++)
	{
		TCHAR ComName[MIN_STR] = _T("");

		StringCchPrintf(ComName, _countof(ComName), _T("Com_VI_Flower%d"), iFlowerPetal);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Flower", ComName, (CComponent**)&m_pVIBufferCom[iFlowerPetal])))
				return E_FAIL;

	}


	//For.Com_Texture
	for (_uint iFlowerPetal = 0; iFlowerPetal < FLOWER_END; iFlowerPetal++)
	{
		TCHAR ComName[MIN_STR] = _T("");

		StringCchPrintf(ComName, _countof(ComName), _T("Com_Texture_Petal%d"), iFlowerPetal);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_TerrainBox", ComName, (CComponent**)&m_pTextureCom[iFlowerPetal])))
			return E_FAIL;

	}

	// For.Com_Transform

	for (_uint iFlowerPetal = 0; iFlowerPetal < FLOWER_END; iFlowerPetal++)
	{
		TCHAR ComName[MIN_STR] = _T("");

		StringCchPrintf(ComName, _countof(ComName), _T("Com_TransformHead%d"), iFlowerPetal);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", ComName, (CComponent**)&m_pTransformCom[iFlowerPetal], &tTransformDesc[iFlowerPetal])))
			return E_FAIL;

		m_pTransformCom[iFlowerPetal]->Set_Rotation(_vec3(0.f, D3DX_PI / FLOWER_END * iFlowerPetal, 0.f));

	}

	return S_OK;
}

CFlower * CFlower::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CFlower* pInstance = new CFlower(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(_T("Failed To Create "), LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFlower::Clone_GameObject(void * pArg)
{
	CFlower* pInstance = new CFlower(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(_T("Failed To Clone CTree"), LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlower::Free()
{
	for (_uint iCnt = 0; iCnt < FLOWER_END; iCnt++)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	CGameObject::Free();
}
