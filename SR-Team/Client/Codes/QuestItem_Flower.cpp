#include "stdafx.h"
#include "..\Headers\QuestItem_Flower.h"
#include "Mouse.h"
#include "FlowerQuest.h"
#include "Player.h"
#include "Inventory.h"
USING(Client)


CQuestItem_Flower::CQuestItem_Flower(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < LOADING_END; i++)
	{
		m_pTransformLoading[i] = nullptr;
		m_pTextureLoading[i] = nullptr;
	}
}

CQuestItem_Flower::CQuestItem_Flower(const CQuestItem_Flower & other)
	: CUIObject(other)
{
}

HRESULT CQuestItem_Flower::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CQuestItem_Flower::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return S_OK;

	return S_OK;
}

_int CQuestItem_Flower::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (pMouse == nullptr)
		return GAMEOBJECT::ERR;

	CFlowerQuest* pFlowerQuest = (CFlowerQuest*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_FlowerQuest", 0);
	if (nullptr == pFlowerQuest)
		return E_FAIL;

	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;

	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (nullptr == pInven)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

	if (m_bGatheringFlower && m_iGatheringFlowerCnt <= 2.f)
		m_bGatheringFlower = false;

	// 조건
	// 1. 퀘스트 하는중이어야 한다
	// 2. 꽃밭 안에 있어야 한다
	if (vPlayerPos.x <= 8.f && vPlayerPos.z <= 40.f && !m_bGatheringFlower && pFlowerQuest->Get_FlowerQuestID() == FLOWER_ON_THE_QUEST)
	{
		if (pManagement->Key_Pressing('G'))
		{
			m_fGatheringTime += _fDeltaTime;
			m_bRenderLoadingBar = true;

			if (m_fGatheringTime >= 1.f)
			{
				m_fGatheringTime = 0.f;

				m_bGatheringFlower = true;
				m_bRenderLoadingBar = false;
				++m_iGatheringFlowerCnt;
				pInven->Get_RewardItem(L"flower");
				PRINT_LOG(L"꽃 획득", LOG::CLIENT);
			}
		}
	}

	return GAMEOBJECT::NOEVENT;
}

_int CQuestItem_Flower::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CQuestItem_Flower::Render_UI()
{
	_matrix matTrans, matWorld;

	if (m_bRenderLoadingBar)
	{
		// 로딩바 배경
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureLoading[0]->Get_TexInfo(0);
		_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

		D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
		matWorld = matTrans;
		m_pSprite->SetTransform(&matWorld);

		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureLoading[0]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(100, 255, 255, 255));

		// 로딩바
		_float fProgress = 1.f - m_fGatheringTime;
		pTexInfo = m_pTextureLoading[1]->Get_TexInfo(0);
		vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

		RECT rc = { 0, 0, (LONG)(pTexInfo->Width * fProgress), (LONG)pTexInfo->Height };
		m_pSprite->Draw((LPDIRECT3DTEXTURE9)m_pTextureLoading[1]->GetTexture(0), &rc, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	return S_OK;
}

HRESULT CQuestItem_Flower::Add_Component()
{
	TCHAR szTextureNameTag[][MAX_STR] =
	{
		L"Component_Texture_FlowerQuest_Loading_BackGround",
		L"Component_Texture_FlowerQuest_Loading_Bar"
	};
	TCHAR szTexture[MIN_STR] = L"Com_TextureWnd%d";
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint i = 0; i < LOADING_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i],
			szCombine, (CComponent**)&m_pTextureLoading[i])))
			return E_FAIL;
	}


	return S_OK;
}

CQuestItem_Flower * CQuestItem_Flower::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CQuestItem_Flower* pInstance = new CQuestItem_Flower(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CQuestItem_Flower", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuestItem_Flower::Clone_GameObject(void * _pArg)
{
	CQuestItem_Flower* pInstance = new CQuestItem_Flower(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CQuestItem_Flower", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuestItem_Flower::Free()
{
	for (_uint i = 0; i < LOADING_END; ++i)
	{
		Safe_Release(m_pTransformLoading[i]);
		Safe_Release(m_pTextureLoading[i]);
	}

	CUIObject::Free();
}
