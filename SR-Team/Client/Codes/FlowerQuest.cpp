#include "stdafx.h"
#include "..\Headers\FlowerQuest.h"
#include "Mouse.h"
#include "Inventory.h"

USING(Client)


CFlowerQuest::CFlowerQuest(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < FLOWER_QUEST_END; i++)
		m_pTextureWnd[i] = nullptr;
	for (_uint i = 0; i < CLEAR_STATE_END; ++i)
		m_pTextureHelp[i] = nullptr;
}

CFlowerQuest::CFlowerQuest(const CFlowerQuest & other)
	: CUIObject(other)
{
}

HRESULT CFlowerQuest::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CFlowerQuest::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return S_OK;
	
	m_tQuestWndCollRt.left = 0;
	m_tQuestWndCollRt.right = 1920;
	m_tQuestWndCollRt.top = 600;
	m_tQuestWndCollRt.bottom = 1000;

	m_eSituation = FLOWER_QUEST_END;

	return S_OK;
}

_int CFlowerQuest::Update_GameObject(_float _fDeltaTime)
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
	case FLOWER_GREETINGS:
		if (pManagement->Key_Down(VK_LBUTTON))
		{
			if (!m_bClear)
				m_eSituation = FLOWER_QUESTION;
			else
				m_eSituation = FLOWER_QUEST_END;
		}
		break;

	case FLOWER_QUESTION:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = FLOWER_ANSWER;
		break;

	case FLOWER_ANSWER:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = FLOWER_QUEST_START;
		break;

	case FLOWER_QUEST_START:
		// 수락
		if (pManagement->Key_Pressing(VK_RETURN))
			m_eSituation = FLOWER_ON_THE_QUEST;	// 퀘스트 하는중
		// 거절
		else if (pManagement->Key_Down(VK_ESCAPE))
			m_eSituation = FLOWER_QUEST_END;	// 처음으로 돌아감
		break;

	case FLOWER_QUEST_CLEAR:
		if (pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = FLOWER_REWARD;
			pInven->Delete_Item(L"flower");
			m_bRenderClear = false;
		}
		break;

	case FLOWER_REWARD:
		// 포션 아이템 획득
		if (!m_bGetReward)
		{
			m_bGetReward = true;
			pInven->Get_RewardItem(L"RewardPotion");
			pInven->Get_RewardItem(L"RewardPotion");
			pInven->Get_RewardItem(L"RewardPotion");
		}

		if (pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = FLOWER_EXTRA_QUESTION;
		break;

	case FLOWER_EXTRA_QUESTION:
		if (pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = FLOWER_FINISH;
		break;

	case FLOWER_QUEST_NOCLEAR:
		if (pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = FLOWER_ON_THE_QUEST;
		break;

	case FLOWER_QUEST_END:
		if (pManagement->Key_Pressing('G'))
			m_eSituation = FLOWER_GREETINGS;
		break;

		// 퀘스트 하는중
	case FLOWER_ON_THE_QUEST:	
		if (3 == pInven->Get_ItemCount(L"flower"))
			m_bRenderClear = true;
		if (pManagement->Key_Down('G'))
		{
			if (3 == pInven->Get_ItemCount(L"flower"))
			{
				m_bClear = true;
				m_eSituation = FLOWER_QUEST_CLEAR;
			}
			else
				m_eSituation = FLOWER_QUEST_NOCLEAR;
		}
		break;

	case FLOWER_FINISH:
		if (pManagement->Key_Down('G'))
			m_eSituation = FLOWER_GREETINGS;
		break;

	default:
		break;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CFlowerQuest::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CFlowerQuest::Render_UI()
{
	if (m_eSituation != FLOWER_QUEST_END && m_eSituation != FLOWER_ON_THE_QUEST && m_eSituation != FLOWER_FINISH)
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

HRESULT CFlowerQuest::Render_HelpWnd()
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
		iIndex = CLEAR;
	else if (m_eSituation == FLOWER_ON_THE_QUEST || m_eSituation == FLOWER_QUEST_START)
		iIndex = NO_CLEAR;

	if (iIndex != -1)
	{
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureHelp[iIndex]->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;
		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		D3DXMatrixTranslation(&matTrans, 1700.f, 200.f, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureHelp[iIndex]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 꽃 개수 출력
		StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d",
			pInven->Get_ItemCount(L"flower"));

		D3DXMatrixScaling(&matScale, 3.f, 3.f, 0.f);
		if (iIndex == NO_CLEAR)
			D3DXMatrixTranslation(&matTrans, 1635.f, 220.f, 0.f);
		else
			D3DXMatrixTranslation(&matTrans, 1635.f, 205.f, 0.f);
		matWorld = matScale * matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
			nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		
	}

	return S_OK;
}

HRESULT CFlowerQuest::Add_Component()
{
	TCHAR szTextureNameTag[][MAX_STR] =
	{
		L"Component_Texture_FlowerQuest_Greeting",
		L"Component_Texture_FlowerQuest_Question",
		L"Component_Texture_FlowerQuest_Answer",
		L"Component_Texture_FlowerQuest_QuestStart",
		L"Component_Texture_FlowerQuest_QuestClear",
		L"Component_Texture_FlowerQuest_Reward",
		L"Component_Texture_FlowerQuest_ExtraQuestion",
		L"Component_Texture_FlowerQuest_QuestNoClear"
	};
	TCHAR szTexture[MIN_STR] = L"Com_TextureWnd%d";
	TCHAR szCombine[MIN_STR] = L"";

	TCHAR szTextureHelpNameTag[][MAX_STR] = 
	{
		L"Component_Texture_FlowerQuest_HelpWnd_NoClear",
		L"Component_Texture_FlowerQuest_HelpWnd_Clear",
	};
	TCHAR szTextureHelp[MIN_STR] = L"Com_TextureHelp%d";
	TCHAR szCombineHelp[MIN_STR] = L"";

	for (_uint i = 0; i < FLOWER_QUEST_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i],
			szCombine, (CComponent**)&m_pTextureWnd[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < CLEAR_STATE_END; i++)
	{
		StringCchPrintf(szCombineHelp, _countof(szCombineHelp), szTextureHelp, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureHelpNameTag[i],
			szCombineHelp, (CComponent**)&m_pTextureHelp[i])))
			return E_FAIL;
	}

	return S_OK;
}

CFlowerQuest * CFlowerQuest::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CFlowerQuest* pInstance = new CFlowerQuest(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CFlowerQuest", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFlowerQuest::Clone_GameObject(void * _pArg)
{
	CFlowerQuest* pInstance = new CFlowerQuest(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CFlowerQuest", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlowerQuest::Free()
{
	for (_uint i = 0; i < FLOWER_QUEST_END; i++)
		Safe_Release(m_pTextureWnd[i]);
	for (_uint i = 0; i < CLEAR_STATE_END; ++i)
		Safe_Release(m_pTextureHelp[i]);

	CUIObject::Free();
}
