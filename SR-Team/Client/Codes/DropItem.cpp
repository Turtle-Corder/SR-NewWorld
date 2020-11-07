#include "stdafx.h"
#include "..\Headers\DropItem.h"

USING(Client)

CDropItem::CDropItem(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CDropItem::CDropItem(const CDropItem & other)
	: CGameObject(other)
{
}

HRESULT CDropItem::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CDropItem::Setup_GameObject(void * pArg)
{
	if(pArg)
	m_vPos = *(_vec3*)pArg;

	m_iRand = rand() % 10 + 1;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CDropItem::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
	{
		//-------------------------------------------------
		// 1 => 2 => 4 => Release;
		//-------------------------------------------------
		return GAMEOBJECT::DEAD;
	}

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	if (FAILED(Move(_fDeltaTime)))
		return 0;

	if (FAILED(Setting_ItemTexture()))
		return GAMEOBJECT::WARN;

	if (FAILED(Setting_ItemBox()))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTextureCom[ITEM_BOX]->Update_Frame(_fDeltaTime, &m_iCurFrame)))
		return GAMEOBJECT::WARN;

	return 0;
}

int CDropItem::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
		return GAMEOBJECT::WARN;

	if(FAILED(pManagement->Add_RendererList(CRenderer::RENDER_ONLYALPHA , this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CDropItem::Render_OnlyAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[ITEM_TEXTURE]->Set_Transform(&m_pTransformCom[ITEM_TEXTURE]->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[ITEM_TEXTURE]->SetTexture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[ITEM_TEXTURE]->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropItem::Render_BlendAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[ITEM_BOX]->Set_Transform(&m_pTransformCom[ITEM_BOX]->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[ITEM_BOX]->SetTexture(m_iCurFrame)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[ITEM_BOX]->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropItem::Add_Component()
{
	TCHAR szName[MAX_PATH] = L"";

	// For.Com_VIBuffer
	switch (m_iRand)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		StringCchPrintf(szName, _countof(szName), 
			L"Component_Texture_DropDiamond");
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		StringCchPrintf(szName, _countof(szName),
			L"Component_Texture_DropRuby");
		break;
	}

	CTransform::TRANSFORM_DESC tTransformDesc[ITEM_END];

	tTransformDesc[ITEM_TEXTURE].vPosition = { 0.f , 0.f , 0.01f };
	tTransformDesc[ITEM_TEXTURE].fSpeedPerSecond = 10.f;
	tTransformDesc[ITEM_TEXTURE].fRotatePerSecond = D3DXToRadian(90.f);

	tTransformDesc[ITEM_BOX].vPosition = { m_vPos.x , m_vPos.y , m_vPos.z };
	tTransformDesc[ITEM_BOX].fSpeedPerSecond = 10.f;
	tTransformDesc[ITEM_BOX].fRotatePerSecond = D3DXToRadian(90.f);


	//----------------------------------------------------
	// ITEM_TEXTURE COMPONENT
	//----------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer0", (CComponent**)&m_pVIBufferCom[ITEM_TEXTURE])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform0", (CComponent**)&m_pTransformCom[ITEM_TEXTURE], &tTransformDesc[ITEM_TEXTURE])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szName, L"Com_Texture0", (CComponent**)&m_pTextureCom[ITEM_TEXTURE])))
		return E_FAIL;

	//----------------------------------------------------
	// ITEM_BOX COMPONENT
	//----------------------------------------------------

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer1", (CComponent**)&m_pVIBufferCom[ITEM_BOX])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform1", (CComponent**)&m_pTransformCom[ITEM_BOX], &tTransformDesc[ITEM_BOX])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Translucent_Cube" , L"Com_Texture1", (CComponent**)&m_pTextureCom[ITEM_BOX])))
		return E_FAIL;

	if (FAILED(m_pTextureCom[ITEM_BOX]->SetFrameRange(0, 1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropItem::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropItem::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPosition = m_pTransformCom[ITEM_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom[ITEM_BASE]->Set_Position(vPosition);
	}

	return S_OK;
}

HRESULT CDropItem::Move(float _fDeltaTime)
{
	m_pTransformCom[ITEM_TEXTURE]->Turn(CTransform::AXIS_Y, _fDeltaTime);

	return S_OK;
}

HRESULT CDropItem::Setting_ItemBox()
{
	if (FAILED(m_pTransformCom[ITEM_BOX]->Update_Transform()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropItem::Setting_ItemTexture()
{
	if (FAILED(m_pTransformCom[ITEM_TEXTURE]->Update_Transform()))
		return E_FAIL;

	m_pTransformCom[ITEM_TEXTURE]->Set_WorldMatrix(m_pTransformCom[ITEM_TEXTURE]->Get_Desc().matWorld * m_pTransformCom[ITEM_BOX]->Get_Desc().matWorld);

	return S_OK;
}

HRESULT CDropItem::Floating(_float _fDeltaTime)
{
	m_fTimeFlow += _fDeltaTime;

	if (m_fTimeFlow >= D3DX_PI)
		m_fTimeFlow = 0;

	for (_uint iCnt = 0; iCnt < ITEM_TEXTURE; ++iCnt)
	{

		_vec3 Temp = m_pTransformCom[iCnt]->Get_Desc().vPosition;

		Temp.y = sinf(m_fTimeFlow);

		m_pTransformCom[iCnt]->Set_Position(Temp);
	}

	return S_OK;
}

CDropItem* CDropItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CDropItem* pInstance = new CDropItem(pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create DropItem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDropItem::Clone_GameObject(void * pArg)
{
	CDropItem* pInstance = new CDropItem(*this);

	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone DropItem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDropItem::Free()
{
	for (_uint iCnt = 0; iCnt < ITEM_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	CGameObject::Free();
}
