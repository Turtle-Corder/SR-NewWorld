#include "stdafx.h"
#include "..\Headers\Quest1.h"

#include "Mouse.h"
#include "Inventory.h"


USING(Client)


CQuest1::CQuest1(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < QUEST1_END; i++)
	{
		m_pTextureCom[i] = nullptr;
	}
}

CQuest1::CQuest1(const CQuest1 & other)
	: CUIObject(other)
{
}

HRESULT CQuest1::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CQuest1::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return S_OK;

	m_tGreetingCollRt.left = 300;
	m_tGreetingCollRt.top = 800;
	m_tGreetingCollRt.right = 932;
	m_tGreetingCollRt.bottom = 930;

	m_tQuestWndCollRt.left = 0;
	m_tQuestWndCollRt.right = 1920;
	m_tQuestWndCollRt.top = 600;
	m_tQuestWndCollRt.bottom = 1000;

	return S_OK;
}

_int CQuest1::Update_GameObject(_float _fDeltaTime)
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
	case GREETINGS:
		if (pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = REQUEST;
		break;

	case REQUEST:
		if (pManagement->Key_Pressing(VK_RETURN))
			m_eSituation = ACCEPT;
		else if (pManagement->Key_Down(VK_ESCAPE))
			m_eSituation = REJECT;
		break;

	case REJECT:
		if (pManagement->Key_Pressing(VK_LBUTTON))
			m_eSituation = QUEST1_END;
		break;

	case ACCEPT:
		if (pManagement->Key_Pressing(VK_LBUTTON))
			m_eSituation = QUEST1_WAIT;
		break;

	case CLEAR:
		if (pManagement->Key_Pressing(VK_LBUTTON))
			m_eSituation = QUEST1_FINISH;
		break;

	case NOCLEAR:
		if (pManagement->Key_Pressing(VK_LBUTTON))
			m_eSituation = QUEST1_WAIT;
		break;

	case QUEST1_END:
		if (pManagement->Key_Pressing('G'))
			m_eSituation = GREETINGS;
		break;

	case QUEST1_WAIT:
		if (pManagement->Key_Pressing('G'))
			m_eSituation = NOCLEAR;
		if (3 == pInven->Get_ItemCount(L"Goguma"))
			m_eSituation = QUEST1_CLEARWAIT;
		break;

	case QUEST1_CLEARWAIT:
		if (pManagement->Key_Pressing('G'))
		{
			pInven->Delete_Item(L"Goguma");
			m_eSituation = CLEAR;
		}
		break;

	case QUEST1_FINISH:
		if (pManagement->Key_Pressing('G'))
			m_eSituation = FINAL;
		break;

	case FINAL:
		if (pManagement->Key_Pressing(VK_LBUTTON))
			m_eSituation = QUEST1_FINISH;
		break;

	default:
		break;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CQuest1::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

HRESULT CQuest1::Render_UI()
{
	if (m_eSituation != QUEST1_END && m_eSituation != QUEST1_WAIT && m_eSituation != QUEST1_CLEARWAIT && m_eSituation != QUEST1_FINISH)
	{
		D3DXMATRIX matTrans, matWorld;
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[m_eSituation]->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;

		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		D3DXMatrixTranslation(&matTrans, 950.f, 800.f, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureCom[m_eSituation]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (FAILED(Render_HelpWnd()))
		return S_OK;

	return S_OK;
}

HRESULT CQuest1::Render_HelpWnd()
{
	TCHAR		szBuff[MAX_PATH] = L"";
	D3DXMATRIX	matScale2, matTrans2, matWorld2;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	if (m_eSituation == QUEST1_WAIT)
	{
		StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d / 3",
			pInven->Get_ItemCount(L"Goguma"));
	}
	else if (m_eSituation == QUEST1_CLEARWAIT)
		StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"Äù½ºÆ® ¿Ï·á!");

	D3DXMatrixScaling(&matScale2, 3.f, 3.f, 0.f);
	D3DXMatrixTranslation(&matTrans2, 1600.f, 400.f, 0.f);
	matWorld2 = matScale2 * matTrans2;

	m_pSprite->SetTransform(&matWorld2);
	m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
		nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));


	return S_OK;
}

HRESULT CQuest1::Add_Component()
{
	TCHAR szTextureNameTag[][MAX_STR] =
	{
		L"Component_Texture_Quest1_One",
		L"Component_Texture_Quest1_Two",
		L"Component_Texture_Quest1_ThreeNo",
		L"Component_Texture_Quest1_ThreeOk",
		L"Component_Texture_Quest1_FourClear",
		L"Component_Texture_Quest1_FourNoClear",
		L"Component_Texture_Quest1_Final"
	};
	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint i = 0; i < QUEST1_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i],
			szCombine, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

CQuest1 * CQuest1::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CQuest1* pInstance = new CQuest1(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CQuest1", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuest1::Clone_GameObject(void * _pArg)
{
	CQuest1* pInstance = new CQuest1(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CQuest1", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuest1::Free()
{
	for (_uint i = 0; i < QUEST1_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}

	CUIObject::Free();
}
