#include "stdafx.h"
#include "..\Headers\Shop_ChatWnd.h"
#include "Shop.h"

USING(Client)


CShop_ChatWnd::CShop_ChatWnd(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < SHOPCHAT_END; i++)
		m_pTextureCom[i] = nullptr;
}

CShop_ChatWnd::CShop_ChatWnd(const CShop_ChatWnd & other)
	: CUIObject(other)
{
}

HRESULT CShop_ChatWnd::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CShop_ChatWnd::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return S_OK;

	m_eSituation = SHOPCHAT_END;

	return S_OK;
}

_int CShop_ChatWnd::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CShop* pShop = (CShop*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Shop");
	if (nullptr == pShop)
		return GAMEOBJECT::ERR;

	if (pManagement->Key_Down(VK_ESCAPE))
	{
		m_eSituation = SHOPCHAT_FINISH;
		m_bStartChat = false;
		// 상점 끄기
		pShop->Set_Render(false);

		return GAMEOBJECT::NOEVENT;
	}

	switch (m_eSituation)
	{
	case SHOPCHAT_GREETINGS:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = SHOPCHAT_REQUIRE;
		else if (pManagement->Key_Down(VK_ESCAPE))
		{
			m_eSituation = SHOPCHAT_FINISH;
			m_bStartChat = false;
		}
		break;

	case SHOPCHAT_REQUIRE:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = SHOPCHAT_ANSWER;
		break;

	case SHOPCHAT_ANSWER:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = SHOPCHAT_FINAL;
		break;

	case SHOPCHAT_FINAL:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = SHOPCHAT_FINISH;
			m_bStartChat = false;

			// 상점 그리기
			pShop->Set_Render(true);
		}
		break;

	case SHOPCHAT_END:
		if (m_bStartChat)
			m_eSituation = SHOPCHAT_GREETINGS;
		break;

	case SHOPCHAT_FINISH:
		if (m_bStartChat)
			m_eSituation = SHOPCHAT_GREETINGS;
		break;

	default:
		break;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CShop_ChatWnd::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CShop_ChatWnd::Render_UI()
{
	if (m_eSituation != SHOPCHAT_END && m_eSituation != SHOPCHAT_FINISH)
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
	return S_OK;
}

HRESULT CShop_ChatWnd::Add_Component()
{
	TCHAR szTextureNameTag[][MAX_STR] =
	{
		L"Component_Texture_Shop_ChatWnd0",
		L"Component_Texture_Shop_ChatWnd1",
		L"Component_Texture_Shop_ChatWnd2",
		L"Component_Texture_Shop_ChatWnd3",
	};
	TCHAR szTexture[MIN_STR] = L"Com_TextureWnd%d";
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint i = 0; i < SHOPCHAT_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i],
			szCombine, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

CShop_ChatWnd * CShop_ChatWnd::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CShop_ChatWnd* pInstance = new CShop_ChatWnd(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CShop_ChatWnd", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShop_ChatWnd::Clone_GameObject(void * _pArg)
{
	CShop_ChatWnd* pInstance = new CShop_ChatWnd(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CShop_ChatWnd", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop_ChatWnd::Free()
{
	for (_uint i = 0; i < SHOPCHAT_END; i++)
		Safe_Release(m_pTextureCom[i]);

	CUIObject::Free();
}
