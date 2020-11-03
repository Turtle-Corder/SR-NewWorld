#include "stdafx.h"
#include "..\Headers\Tree.h"

USING(Client)

CTree::CTree(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	for (int i = 0; i < TREE_END; i++)
	{
		m_pVIBufferCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}
}

CTree::CTree(const CTree & other)
	: CGameObject(other)
{
}

HRESULT CTree::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CTree::Setup_GameObject(void * _pArg)
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

int CTree::Update_GameObject(float DeltaTime)
{
	for (_uint iCnt = 0; iCnt < TREE_END; iCnt++)
	{
		if (FAILED(m_pTransformCom[iCnt]->Update_Transform()))
			return GAMEOBJECT::WARN;
	}


	return GAMEOBJECT::NOEVENT;
}

int CTree::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return 0;
}

HRESULT CTree::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int i = 0; i < BODY_ID::TREE_END; i++)
	{
		if (FAILED(m_pVIBufferCom[i]->Set_Transform(&m_pTransformCom[i]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[i]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[i]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTree::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc[TREE_END];

	return S_OK;
}

HRESULT CTree::Add_Component(_vec3 _vPos)
{
	_vec3 vPositon_Head = _vPos + _vec3(0.f, 1.f, 0.f);
	_vec3 vPositon_Body = _vPos + _vec3(0.f, 0.5f, 0.f);
	CTransform::TRANSFORM_DESC tTransformDesc[TREE_END];
	tTransformDesc[TREE_BODY].vPosition = vPositon_Body;
	tTransformDesc[TREE_BODY].vScale = { 0.5f, 1.f, 0.5f };
	tTransformDesc[TREE_HEAD].vPosition = vPositon_Head;

	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Pyramid", L"Com_VIBufferHead", (CComponent**)&m_pVIBufferCom[TREE_HEAD])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBufferBody", (CComponent**)&m_pVIBufferCom[TREE_BODY])))
		return E_FAIL;


	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Monster", L"Com_TextureHead", (CComponent**)&m_pTextureCom[TREE_HEAD])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Monster", L"Com_TextureBody", (CComponent**)&m_pTextureCom[TREE_BODY])))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformHead", (CComponent**)&m_pTransformCom[TREE_HEAD], &tTransformDesc[TREE_HEAD])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformBody", (CComponent**)&m_pTransformCom[TREE_BODY], &tTransformDesc[TREE_BODY])))
		return E_FAIL;



	return S_OK;
}

CTree * CTree::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CTree* pInstance = new CTree(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(_T("Failed To Create CTree"), LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTree::Clone_GameObject(void * pArg)
{
	CTree* pInstance = new CTree(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(_T("Failed To Clone CTree"), LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTree::Free()
{
	for (int i = 0; i < 2; i++)
	{
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
		Safe_Release(m_pTextureCom[i]);
	}

	CGameObject::Free();
}
