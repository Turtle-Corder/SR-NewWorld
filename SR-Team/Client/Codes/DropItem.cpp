#include "stdafx.h"
#include "DamageInfo.h"
#include "Player.h"
#include "Inventory.h"
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
		m_tBoxInfo = *(DROPBOX_INFO*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CDropItem::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

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

	Update_DeadDelay(_fDeltaTime);

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

HRESULT CDropItem::Take_Damage(const CComponent * _pDamageComp)
{
	CDamageInfo* pDmgInfo = (CDamageInfo*)_pDamageComp;
	CPlayer* pPlayer = (CPlayer*)pDmgInfo->Get_Desc().pOwner;
	if (pPlayer && pPlayer->IsInteraction())
	{
		// 여기에 죽기 전에 인벤에 추가
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return GAMEOBJECT::WARN;
		CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
		if (nullptr == pInven)
			return GAMEOBJECT::WARN;

		if (m_tBoxInfo.iItemNo == 0)
			pInven->Get_RewardItem(L"Diamond");
		else if (m_tBoxInfo.iItemNo == 1)
			pInven->Get_RewardItem(L"GolemCore_Green");

		m_bDead = true;
	}

	return S_OK;
}

HRESULT CDropItem::Add_Component()
{
	TCHAR szName[MIN_STR] = L"";

	if (0 == m_tBoxInfo.iItemNo)
		StringCchPrintf(szName, _countof(szName), L"Component_Texture_DropDiamond");
	else if (1 == m_tBoxInfo.iItemNo)
		StringCchPrintf(szName, _countof(szName), L"Component_Texture_MainQuest_HelpWnd_GolemCore_Green");

	// For.Com_VIBuffer
	CTransform::TRANSFORM_DESC tTransformDesc[ITEM_END];

	tTransformDesc[ITEM_TEXTURE].vPosition = { 0.f , 0.f , 0.01f };
	tTransformDesc[ITEM_TEXTURE].fSpeedPerSecond = 10.f;
	tTransformDesc[ITEM_TEXTURE].fRotatePerSecond = D3DXToRadian(90.f);

	tTransformDesc[ITEM_BOX].vPosition = m_tBoxInfo.vPos;
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

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_DropItem" , L"Com_Texture1", (CComponent**)&m_pTextureCom[ITEM_BOX])))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc[ITEM_BOX].vPosition;
	tColDesc.fRadius = 0.5f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
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

void CDropItem::Update_DeadDelay(_float _fDeltaTime)
{
	if (m_tBoxInfo.bGone)
	{
		m_fDeadTime += _fDeltaTime;
		if(m_fDeadTime >= 8.f)
			m_bDead = true;
	}
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

	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
