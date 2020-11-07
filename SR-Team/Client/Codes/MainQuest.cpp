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
		// 수락
		if (pManagement->Key_Pressing(VK_RETURN))
			m_eSituation = MAINQUEST_AGREE;
		else if (pManagement->Key_Pressing(VK_ESCAPE))
			m_eSituation = MAINQUEST_REJECT;
		break;

	case MAINQUEST_AGREE:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = MAINQUEST_ON_THE_QUEST;	// 퀘스트 하는중
		break;

	case MAINQUEST_REJECT:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = MAINQUEST_END;
		break;

	case MAINQUEST_REWARD:
		// 보상 추가해야함
		
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
	return S_OK;
}

HRESULT CMainQuest::Add_Component()
{
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

	CUIObject::Free();
}
