#include "stdafx.h"
#include "..\Headers\MainQuest.h"
#include "Mouse.h"
#include "Inventory.h"

USING(Client)

CMainQuest::CMainQuest(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < MAINQUEST_END; ++i)
		m_pTextureWnd[i] = nullptr;
	for (_uint i = 0; i < MAIN_STATE_END; ++i)
		m_pTextureHelp[i] = nullptr;
}

CMainQuest::CMainQuest(const CMainQuest & other)
	: CUIObject(other)
{
}

HRESULT CMainQuest::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMainQuest::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CMainQuest::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (pMouse == nullptr)
		return E_FAIL;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	if (FAILED(Check_GolemCore_Count()))
		return GAMEOBJECT::WARN;

	switch (m_eSituation)
	{
	case MAINQUEST_GREETINGS:
		if (pManagement->Key_Down(VK_LBUTTON) || pManagement->Key_Down(VK_SPACE))
		{
			if (!m_bClear)
				m_eSituation = MAINQUEST_ASK1;
			else
				m_eSituation = MAINQUEST_END;
		}
		if (pManagement->Key_Pressing(VK_ESCAPE))
		{
			m_eSituation = MAINQUEST_FINISH;
			m_bStartQuest = false;
		}
		break;

	case MAINQUEST_ASK1:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = MAINQUEST_ANSWER1;
		break;

	case MAINQUEST_ANSWER1:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = MAINQUEST_ASK2;
		break;

	case MAINQUEST_ASK2:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = MAINQUEST_ANSWER2;
		break;

	case MAINQUEST_ANSWER2:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = MAINQUEST_ASK_QUEST;
		break;

	case MAINQUEST_ASK_QUEST:
		// ¼ö¶ô
		if (pManagement->Key_Pressing(VK_RETURN))
			m_eSituation = MAINQUEST_AGREE;
		else if (pManagement->Key_Pressing(VK_ESCAPE))
			m_eSituation = MAINQUEST_REJECT;
		break;

	case MAINQUEST_AGREE:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = MAINQUEST_ON_THE_QUEST;	// Äù½ºÆ® ÇÏ´ÂÁß
			m_bStartQuest = false;
		}
		break;

	case MAINQUEST_REJECT:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = MAINQUEST_END;
			m_bStartQuest = false;
		}
		break;

	case MAINQUEST_REWARD:
		// º¸»ó Ãß°¡ÇØ¾ßÇÔ
		
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = MAINQUEST_FINISH;
			m_bStartQuest = false;
		}
		break;

	case MAINQUEST_END:
		if (m_bStartQuest)
			m_eSituation = MAINQUEST_GREETINGS;
		break;

	case MAINQUEST_ON_THE_QUEST:
		if (4 == pInven->Get_ItemCount(L"Golem"))
			m_bRenderClear = true;
		if (m_bStartQuest)
		{
			if (4 == pInven->Get_ItemCount(L"Golem"))
			{
				m_bClear = true;
				m_eSituation = MAINQUEST_REWARD;
				pInven->Delete_Item(L"Golem");
				m_bRenderClear = false;
			}
			else
			{
				m_eSituation = MAINQUEST_NO_CLEAR;
				m_bStartQuest = false;
			}
		}
		break;

	case MAINQUEST_NO_CLEAR:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = MAINQUEST_ON_THE_QUEST;
		break;

	case MAINQUEST_FINISH:
		if (m_bStartQuest)
			m_eSituation = MAINQUEST_GREETINGS;
		break;

	default:
		break;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CMainQuest::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMainQuest::Render_UI()
{
	if (m_eSituation != MAINQUEST_END && m_eSituation != MAINQUEST_ON_THE_QUEST && m_eSituation != MAINQUEST_FINISH)
	{
		D3DXMATRIX matTrans, matWorld;
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureWnd[m_eSituation]->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;

		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		D3DXMatrixTranslation(&matTrans, 950.f, 800.f, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureWnd[m_eSituation]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	if (FAILED(Render_HelpWnd()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainQuest::Render_HelpWnd()
{
	_int iIndex = -1;
	TCHAR szBuff[MIN_STR] = L"";
	_matrix matTrans, matWorld, matScale;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	if (m_bRenderClear)
		iIndex = MAIN_STATE_CLEAR;
	else if (m_eSituation == MAINQUEST_ON_THE_QUEST)
		iIndex = MAIN_STATE_NOCLEAR;

	if (iIndex != -1)
	{
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureHelp[iIndex]->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;
		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		D3DXMatrixTranslation(&matTrans, 1700.f, 400.f, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureHelp[iIndex]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// °ñ·½ ÇÙ °³¼ö Ãâ·Â
		// »¡°­ -> ÃÊ·Ï -> ÆÄ¶û -> ¹àÀº ÆÄ¶û
		for (_uint i = 0; i < GOLEM_SORT_END; i++)
		{
			if (m_bGetGolemCore[i])
				StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"1");
			else
				StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"0");

			D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
			if (iIndex == MAIN_STATE_NOCLEAR)
				D3DXMatrixTranslation(&matTrans, 1722.f, 371.f + (i * 25.f), 0.f);
			else
				D3DXMatrixTranslation(&matTrans, 1722.f, 341.f + (i * 25.f), 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
				nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		}
	}

	return S_OK;
}

HRESULT CMainQuest::Add_Component()
{
	TCHAR szTextureNameTag[][MAX_STR] =
	{
		L"Component_Texture_MainQuest_One",
		L"Component_Texture_MainQuest_Two",
		L"Component_Texture_MainQuest_Three",
		L"Component_Texture_MainQuest_Four",
		L"Component_Texture_MainQuest_Five",
		L"Component_Texture_MainQuest_Six",
		L"Component_Texture_MainQuest_Seven",
		L"Component_Texture_MainQuest_Reject",
		L"Component_Texture_MainQuest_ing",
		L"Component_Texture_MainQuest_Reward"
	};
	TCHAR szTexture[MIN_STR] = L"Com_TextureWnd%d";
	TCHAR szCombine[MIN_STR] = L"";

	TCHAR szTextureHelpNameTag[][MAX_STR] =
	{
		L"Component_Texture_MainQuest_HelpWnd_NoClear",
		L"Component_Texture_MainQuest_HelpWnd_Clear",
	};
	TCHAR szTextureHelp[MIN_STR] = L"Com_TextureHelp%d";
	TCHAR szCombineHelp[MIN_STR] = L"";

	for (_uint i = 0; i < MAINQUEST_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i],
			szCombine, (CComponent**)&m_pTextureWnd[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < MAIN_STATE_END; i++)
	{
		StringCchPrintf(szCombineHelp, _countof(szCombineHelp), szTextureHelp, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureHelpNameTag[i],
			szCombineHelp, (CComponent**)&m_pTextureHelp[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainQuest::Check_GolemCore_Count()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	for (_uint i = 0; i < GOLEM_SORT_END; i++)
	{
		if (i == RED)
		{
			if (1 == pInven->Get_ItemCount(L"GolemCore_Red"))
				m_bGetGolemCore[i] = true;
			else if (0 == pInven->Get_ItemCount(L"GolemCore_Red"))
				m_bGetGolemCore[i] = false;
		}
		else if (i == GREEN)
		{
			if (1 == pInven->Get_ItemCount(L"GolemCore_Green"))
				m_bGetGolemCore[i] = true;
			else if (0 == pInven->Get_ItemCount(L"GolemCore_Green"))
				m_bGetGolemCore[i] = false;
		}
		else if (i == BLUE)
		{
			if (1 == pInven->Get_ItemCount(L"GolemCore_Blue"))
				m_bGetGolemCore[i] = true;
			else if (0 == pInven->Get_ItemCount(L"GolemCore_Blue"))
				m_bGetGolemCore[i] = false;
		}
		else if (i == BRIGHT_BLUE)
		{
			if (1 == pInven->Get_ItemCount(L"GolemCore_BrightBlue"))
				m_bGetGolemCore[i] = true;
			else if (0 == pInven->Get_ItemCount(L"GolemCore_BrightBlue"))
				m_bGetGolemCore[i] = false;
		}
	}

	return S_OK;
}

CMainQuest * CMainQuest::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CMainQuest* pInstance = new CMainQuest(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMainQuest", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMainQuest::Clone_GameObject(void * _pArg)
{
	CMainQuest* pInstance = new CMainQuest(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMainQuest", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest::Free()
{
	for (_uint i = 0; i < MAINQUEST_END; ++i)
		Safe_Release(m_pTextureWnd[i]);

	for (_uint i = 0; i < MAIN_STATE_END; ++i)
		Safe_Release(m_pTextureHelp[i]);

	CUIObject::Free();
}
