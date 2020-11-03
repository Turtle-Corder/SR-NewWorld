#include "stdafx.h"
#include "DataManager.h"
#include "UICamera.h"
#include "Inventory.h"
#include "Mouse.h"
#include "..\Headers\Shop.h"

USING(Client)

CShop::CShop(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < SHOP_END; i++)
	{
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}

	for (_uint i = 0; i < 16; ++i)
	{
		m_pItemTransformCom[i] = nullptr;
		m_pItemTextureCom[i] = nullptr;
	}
}

CShop::CShop(const CShop & other)
	: CUIObject(other)
{
}

void CShop::Set_ShopItemTexturePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemTexturePos, sizeof(m_vItemTexturePos));
}

void CShop::Set_ShopItemNamePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemNamePos, sizeof(m_vItemNamePos));
}

void CShop::Set_ShopItemPricePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemPricePos, sizeof(m_vItemPricePos));
}

HRESULT CShop::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CShop::Setup_GameObject(void * pArg)
{
	m_vShopItem.reserve(10);

	if (FAILED(Add_Component()))
		return E_FAIL;
	if (FAILED(Add_Component_ShopItem()))
		return E_FAIL;

	// 변경
	m_pTransformCom[SHOP_WND]->Set_Position(_vec3(400.f, 400.f, 0.f));
	m_pTransformCom[SHOP_SCROLLBAR]->Set_Position(_vec3(810.f, 230.f, 0.f));

	// 아이템 텍스처, 이름, 가격 위치 설정
	_uint iIndex = 0;
	// 변경
	for (_uint j = 0; j < 3; ++j)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			iIndex = j * 4 + i;
			m_vItemTexturePos[j][i].x = (i * 170.f) + 208.f;
			m_vItemTexturePos[j][i].y = (j * 213.f) + 156.f;
			m_vItemTexturePos[j][i].z = 0.f;

			if (iIndex < 16)
			{
				m_pItemTransformCom[iIndex]->Set_Position(m_vItemTexturePos[j][i]);
			}
			int k = 0;
		}
	}

	// 아이템 이미지 충돌 렉트
	// 변경
	for (_uint j = 0; j < 3; ++j)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			iIndex = j * 4 + i;
			if (iIndex >= 16)
				return S_OK;
			m_tItemTextureRt[j][i].left = (LONG)(m_vItemTexturePos[j][i].x - 20.f);
			m_tItemTextureRt[j][i].right = (LONG)(m_vItemTexturePos[j][i].x + 20.f);
			m_tItemTextureRt[j][i].top = (LONG)(m_vItemTexturePos[j][i].y - 20.f);
			m_tItemTextureRt[j][i].bottom = (LONG)(m_vItemTexturePos[j][i].y + 20.f);
		}
	}

	return S_OK;
}

int CShop::Update_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if ((pManagement->Key_Down('O')))
		m_bRender = !m_bRender;

	if (m_bRender)
	{
		if (FAILED(Check_BuyItem()))
			return GAMEOBJECT::WARN;
		if (FAILED(Move_ScrollBar()))
			return GAMEOBJECT::WARN;

		for (_uint i = 0; i < SHOP_END; ++i)
		{
			m_pTransformCom[i]->Update_Transform();
		}
		for (_uint i = 0; i < 15; ++i)
		{
			m_pItemTransformCom[i]->Update_Transform();
		}
	}

	return GAMEOBJECT::NOEVENT;
}

int CShop::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CShop::Render_UI()
{
	if (m_bRender)
	{
		m_DC = GetDC(g_hWnd);

		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		// 상점 윈도우 그리기
		for (_uint i = 0; i < SHOP_END; ++i)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			_vec3 vPos = m_pTransformCom[i]->Get_Desc().vPosition;

			m_tShopWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tShopWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tShopWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tShopWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pTransformCom[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (FAILED(Render_ShopItem()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShop::Check_BuyItem()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return GAMEOBJECT::NOEVENT;

	for (_int j = m_iStartIndex; j < 3 + m_iStartIndex; ++j)
	{
		for (_int i = 0; i < 4; ++i)
		{
			if (pManagement->Key_Pressing(VK_LBUTTON))
			{
				if (PtInRect(&m_tItemTextureRt[j][i], pMouse->Get_Point()))
					Buy_Item(j, i);
			}
		}
	}

	return S_OK;
}

HRESULT CShop::Buy_Item(_uint iIndexJ, _uint iIndexI)
{
	int iIndex = iIndexJ * 4 + iIndexI;
	wstring strName = m_vShopItem[iIndex]->szItemTag;

	CManagement* pManagement = CManagement::Get_Instance();
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	pInven->Get_ShopItem(strName);

	return S_OK;
}

HRESULT CShop::Move_ScrollBar()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (pManagement->Key_Pressing(VK_LBUTTON))
	{
		CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
		if (nullptr == pMouse)
			return S_OK;

		_vec3 vScrollBarPos = m_pTransformCom[SHOP_SCROLLBAR]->Get_Desc().vPosition;

		if (PtInRect(&m_tShopWndCollRt[SHOP_SCROLLBAR], pMouse->Get_Point()))
		{
			vScrollBarPos.y = (_float)pMouse->Get_Point().y;


			if (vScrollBarPos.y >= 580.f)
			{
				vScrollBarPos.y = 580.f;
				m_iStartIndex = 5;
			}
			else if (vScrollBarPos.y >= 520.f)
				m_iStartIndex = 5;
			else if (vScrollBarPos.y >= 460.f)
				m_iStartIndex = 4;
			else if (vScrollBarPos.y >= 400.f)
				m_iStartIndex = 3;
			else if (vScrollBarPos.y >= 340.f)
				m_iStartIndex = 2;
			else if (vScrollBarPos.y >= 280.f)
				m_iStartIndex = 1;
			else if (vScrollBarPos.y > 220.f)
				m_iStartIndex = 0;
			else if (vScrollBarPos.y <= 220.f)
			{
				vScrollBarPos.y = 220.f;
				m_iStartIndex = 0;
			}
		}

		m_pTransformCom[SHOP_SCROLLBAR]->Set_Position(vScrollBarPos);
	}
	return S_OK;
}

HRESULT CShop::Add_Component()
{
	for (_uint i = 0; i < SHOP_END; ++i)
	{
		// 2. Transform
		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		if (i == SHOP_WND)
			wsprintf(szTextureName, L"Component_Texture_Shop_ShopWnd");
		else if (i == SHOP_SCROLLBAR)
			wsprintf(szTextureName, L"Component_Texture_Shop_ShopScrollBar");
		wsprintf(szTexture, L"Com_Texture%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShop::Add_Component_ShopItem()
{
	int iIndex = 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CDataManager* pOrigin = (CDataManager*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Item");
	if (pOrigin == nullptr)
		return E_FAIL;

	for (_uint i = 0; i < 16; ++i)
	{
		// Transform-----------------------------------------------------------------
		TCHAR szItemTransform[MAX_STR] = L"";
		wsprintf(szItemTransform, L"Com_ShopItemTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform",
			szItemTransform, (CComponent**)&m_pItemTransformCom[i])))
			return E_FAIL;

		// Texture-------------------------------------------------------------------
		TCHAR szItemTexture[MAX_STR] = L"";
		TCHAR szItemTextureName[MAX_STR] = L"";
		INVEN_ITEM* pItem = new INVEN_ITEM;
		if (i == 0)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"GoldSword");
			wsprintf(szItemTextureName, L"Component_Texture_Item_GoldenSword");
		}
		else if (i == 1)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"IronSword");
			wsprintf(szItemTextureName, L"Component_Texture_Item_IronSword");
		}
		else if (i == 2)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"DiaSword");
			wsprintf(szItemTextureName, L"Component_Texture_Item_DiaSword");
		}
		else if (i == 3)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BlackDress");
			wsprintf(szItemTextureName, L"Component_Texture_Item_BlackDress");
		}
		else if (i == 4)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"PupleDress");
			wsprintf(szItemTextureName, L"Component_Texture_Item_PupleDress");
		}
		else if (i == 5)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"ScholarShoes");
			wsprintf(szItemTextureName, L"Component_Texture_Item_ScholarShoes");
		}
		else if (i == 6)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"ArcaneShoes");
			wsprintf(szItemTextureName, L"Component_Texture_Item_ArcaneShoes");
		}
		else if (i == 7)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"RedPotion");
			wsprintf(szItemTextureName, L"Component_Texture_Item_RedPotion");
		}
		else if (i == 8)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BluePotion");
			wsprintf(szItemTextureName, L"Component_Texture_Item_BluePotion");
		}
		else if (i == 9)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"RedElixir");
			wsprintf(szItemTextureName, L"Component_Texture_Item_RedElixir");
		}
		else if (i == 10)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BlueElixir");
			wsprintf(szItemTextureName, L"Component_Texture_Item_BlueElixir");
		}
		else if (i == 11)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"AbsoluteBelt");
			wsprintf(szItemTextureName, L"Component_Texture_Item_AbsoluteBelt");
		}
		else if (i == 12)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"AquaGloves");
			wsprintf(szItemTextureName, L"Component_Texture_Item_AquaGloves");
		}
		else if (i == 13)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BalrogWings");
			wsprintf(szItemTextureName, L"Component_Texture_Item_BalrogWings");
		}
		else if (i == 14)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"SorcererGloves");
			wsprintf(szItemTextureName, L"Component_Texture_Item_SorcererGloves");
		}
		else if (i == 15)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"flower");
			wsprintf(szItemTextureName, L"Component_Texture_FlowerQuest_Flower");
		}
		pOrigin->Get_ItemInfo(pItem->szItemTag, *pItem);
		//m_pItemTextureCom[i] = pOrigin->Get_ItemInfo_Texture(pItem->szItemTag);
		m_vShopItem.emplace_back(pItem);
		wsprintf(szItemTexture, L"Com_ShopItemTexture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szItemTextureName,
			szItemTexture, (CComponent**)&m_pItemTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShop::Render_ShopItem()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	D3DXMATRIX	matScale, matTrans, matWorld;

	//m_vItemTexturePos[j][i].x = (i * 115.f) + 270.f;
	_int iIndex = 0;
	for (_int j = m_iStartIndex, k = 0; j < 3 + m_iStartIndex; ++j, ++k)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			iIndex = j * 4 + i;
			if (iIndex >= 16)
				return S_OK;

			// 아이템 이미지--------------------------------------------------------------------------------------
			const D3DXIMAGE_INFO* pTexInfo = m_pItemTextureCom[iIndex]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			//_vec3 vPos = m_pItemTransformCom[iIndex]->Get_Desc().vPosition;
			_vec3 vPos = m_vItemTexturePos[k][i];
			m_pItemTransformCom[iIndex]->Set_Position(vPos);

			m_tItemTextureRt[j][i].left = (LONG)(vPos.x - vCenter.x);
			m_tItemTextureRt[j][i].right = (LONG)(vPos.x + vCenter.x);
			m_tItemTextureRt[j][i].top = (LONG)(vPos.y - vCenter.y);
			m_tItemTextureRt[j][i].bottom = (LONG)(vPos.y + vCenter.y);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&/*m_pItemTransformCom[iIndex]->Get_Desc().*/matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pItemTextureCom[iIndex]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


			// 아이템 이름
			TCHAR		szBuff[MAX_PATH] = L"";
			D3DXMatrixIdentity(&matWorld);
			StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%s", m_vShopItem[iIndex]->szShopTag);

			D3DXMatrixScaling(&matScale, 1.5f, 1.7f, 0.f);
			D3DXMatrixTranslation(&matTrans, vPos.x - 55.f, vPos.y + 55.f, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
				nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


			// 아이템 가격
			TCHAR szBuff2[MAX_PATH] = L"";
			matScale, matTrans, matWorld;
			D3DXMatrixIdentity(&matWorld);
			StringCchPrintf(szBuff2, _countof(szBuff2), L"%d", m_vShopItem[iIndex]->iPrice);

			D3DXMatrixScaling(&matScale, 1.5f, 1.7f, 0.f);
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y + 85.f, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pFont->DrawTextW(m_pSprite, szBuff2, lstrlen(szBuff2),
				nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	int l = 0;
	return S_OK;
}

CShop * CShop::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CShop* pInstance = new CShop(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CShop", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShop::Clone_GameObject(void * pArg)
{
	CShop* pInstance = new CShop(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CShop", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop::Free()
{
	for (_uint i = 0; i < SHOP_END; ++i)
	{
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pTextureCom[i]);
	}
	

	for (_uint i = 0; i < 16; ++i)
	{
		Safe_Release(m_pItemTransformCom[i]);
		Safe_Release(m_pItemTextureCom[i]);
	}

	for (auto& pItem : m_vShopItem)
		Safe_Delete(pItem);
	m_vShopItem.clear();

	CUIObject::Free();
}
