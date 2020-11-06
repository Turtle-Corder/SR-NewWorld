#include "stdafx.h"
#include "..\Headers\QuestItem_Flower.h"
#include "Mouse.h"

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

	if (pManagement->Key_Down(VK_LBUTTON))
	{
		TCHAR szBuff[MIN_STR] = L"";
		wsprintf(szBuff, L"%f, %f", pMouse->Get_Point().x, pMouse->Get_Point().y);
		//PRINT_LOG(szBuff, LOG::CLIENT);
	}

	// 조건
	// 1. 퀘스트 하는중이어야 한다
	// 2. 꽃밭 안에 있어야 한다

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
	if (m_bRenderLoadingBar)
	{
		for (_uint i = 0; i < LOADING_END; i++)
		{

		}
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
