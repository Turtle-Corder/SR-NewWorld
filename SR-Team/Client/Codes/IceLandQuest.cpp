#include "stdafx.h"
#include "..\Headers\IceLandQuest.h"
#include "Inventory.h"

USING(Client)

CIceLandQuest::CIceLandQuest(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < ICEQUEST_END; i++)
		m_pTextureWnd[i] = nullptr;
	for (_uint i = 0; i < ICEQUEST_STATE_END; ++i)
		m_pTextureHelp[i] = nullptr;
}

CIceLandQuest::CIceLandQuest(const CIceLandQuest & other)
	: CUIObject(other)
{
}

void CIceLandQuest::Dead_Monster()
{
	m_iMonsetDeadCnt += 1;
}

HRESULT CIceLandQuest::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CIceLandQuest::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eSituation = ICEQUEST_END;

	return S_OK;
}

_int CIceLandQuest::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	//// 치트 -> 나중에 수정해야 함
	//if (pManagement->Key_Pressing('V'))
	//{
	//	pInven->Get_RewardItem(L"GolemCore_Red");
	//	m_iMonsetDeadCnt = 10;
	//}

	switch (m_eSituation)
	{
	case ICEQUEST_ASK1:
		if (pManagement->Key_Down(VK_LBUTTON))
		{
			if (!m_bClear)
				m_eSituation = ICEQUEST_ANSWER1;
			else
			{
				m_eSituation = ICEQUEST_END;
				m_bStartQuest = false;
			}
		}
		if (pManagement->Key_Pressing(VK_ESCAPE))
		{
			m_eSituation = ICEQUEST_FINISH;
			m_bStartQuest = false;
		}
		break;

	case ICEQUEST_ANSWER1:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = ICEQUEST_ASK2;
		break;

	case ICEQUEST_ASK2:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = ICEQUEST_ANSWER2;
		break;

	case ICEQUEST_ANSWER2:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = ICEQUEST_ASK_QUEST;
		break;

	case ICEQUEST_ASK_QUEST:
		// 수락
		if (pManagement->Key_Pressing(VK_RETURN))
		{
			m_eSituation = ICEQUEST_AGREE;	// 퀘스트 하는중
			m_bStartDeadCnt = true;
		}
		else if (pManagement->Key_Pressing(VK_ESCAPE))
			m_eSituation = ICEQUEST_REJECT;
		break;

	case ICEQUEST_AGREE:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_bStartQuest = false;
			m_eSituation = ICEQUEST_ON_THE_QUEST;
		}
		break;

	case ICEQUEST_REJECT:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = ICEQUEST_END;
			m_bStartQuest = false;
		}
		break;

	case ICEQUEST_REWARD:
		m_bStartDeadCnt = false;
		if (!m_bGetReward)
		{
			m_bGetReward = true;
			pInven->Get_RewardItem(L"GolemCore_Blue");
		}
		else if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = ICEQUEST_FINISH;
			m_bStartQuest = false;
		}

		pManagement->Set_SceneEvent(eSceneEventID::EVENT_CLEAR);
		break;

	case ICEQUEST_NO_CLEAR:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_bStartQuest = false;
			m_eSituation = ICEQUEST_ON_THE_QUEST;
		}
		break;

	case ICEQUEST_END:
		if (m_bStartQuest)
			m_eSituation = ICEQUEST_ASK1;
		break;

	case ICEQUEST_ON_THE_QUEST:
		if (5 == m_iMonsetDeadCnt)
			m_bRenderClear = true;
		if (m_bStartQuest)
		{
			if (5 == m_iMonsetDeadCnt)
			{
				m_bClear = true;
				m_eSituation = ICEQUEST_REWARD;
				m_bRenderClear = false;
			}
			else
			{
				m_eSituation = ICEQUEST_NO_CLEAR;
				m_bStartQuest = false;
			}
		}
		break;

	case ICEQUEST_FINISH:
		if (m_bStartQuest)
		{
			m_eSituation = ICEQUEST_ASK1;
			m_bStartQuest = false;
		}
		break;

	case ICEQUEST_REAL_FINISH:
		break;
	default:
		break;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CIceLandQuest::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CIceLandQuest::Render_UI()
{
	if (m_eSituation != ICEQUEST_END && m_eSituation != ICEQUEST_ON_THE_QUEST && m_eSituation != FLOWER_FINISH && m_eSituation != ICEQUEST_FINISH)
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

		m_bChat = true;
	}
	else
		m_bChat = false;

	if (FAILED(Render_HelpWnd()))
		return E_FAIL;

	return S_OK;
}

HRESULT CIceLandQuest::Render_HelpWnd()
{
	_int iIndex = -1;
	TCHAR szBuff[MIN_STR] = L"";
	_matrix matTrans, matWorld, matScale;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (m_eSituation == ICEQUEST_ON_THE_QUEST)
		iIndex = ICEQUEST_NOCLEAR;
	if (m_bRenderClear)
		iIndex = ICEQUEST_CLEAR;

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

		if (!m_bRenderClear)
		{
			// 처치한 몬스터 수 출력
			StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d", m_iMonsetDeadCnt);

			D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
			if (iIndex == ICEQUEST_NOCLEAR)
				D3DXMatrixTranslation(&matTrans, 1720.f, 232.f, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
				nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		}

	}

	return S_OK;
}

HRESULT CIceLandQuest::Add_Component()
{
	TCHAR szTextureNameTag[][MAX_STR] =
	{
		L"Component_Texture_IceQuest_One",
		L"Component_Texture_IceQuest_Two",
		L"Component_Texture_IceQuest_Three",
		L"Component_Texture_IceQuest_Four",
		L"Component_Texture_IceQuest_Five",

		L"Component_Texture_IceQuest_Agree",
		L"Component_Texture_IceQuest_Reject",
		L"Component_Texture_IceQuest_Reward",

		L"Component_Texture_IceQuest_ing"
	};
	TCHAR szTexture[MIN_STR] = L"Com_TextureWnd%d";
	TCHAR szCombine[MIN_STR] = L"";

	TCHAR szTextureHelpNameTag[][MAX_STR] =
	{
		L"Component_Texture_IceQuest_HelpWnd_NoClear",
		L"Component_Texture_IceQuest_HelpWnd_Clear",
	};
	TCHAR szTextureHelp[MIN_STR] = L"Com_TextureHelp%d";
	TCHAR szCombineHelp[MIN_STR] = L"";

	for (_uint i = 0; i < ICEQUEST_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i],
			szCombine, (CComponent**)&m_pTextureWnd[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < ICEQUEST_STATE_END; i++)
	{
		StringCchPrintf(szCombineHelp, _countof(szCombineHelp), szTextureHelp, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureHelpNameTag[i],
			szCombineHelp, (CComponent**)&m_pTextureHelp[i])))
			return E_FAIL;
	}

	return S_OK;
}

CIceLandQuest * CIceLandQuest::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CIceLandQuest* pInstance = new CIceLandQuest(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CIceLandQuest", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CIceLandQuest::Clone_GameObject(void * _pArg)
{
	CIceLandQuest* pInstance = new CIceLandQuest(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CIceLandQuest", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceLandQuest::Free()
{
	for (_uint i = 0; i < ICEQUEST_END; i++)
		Safe_Release(m_pTextureWnd[i]);
	for (_uint i = 0; i < ICEQUEST_STATE_END; ++i)
		Safe_Release(m_pTextureHelp[i]);

	CUIObject::Free();
}
