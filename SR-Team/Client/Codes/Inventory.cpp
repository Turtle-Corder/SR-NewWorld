#include "stdafx.h"
#include "DataManager.h"
#include "UICamera.h"
#include "Equip.h"
#include "MainUI.h"
#include "Mouse.h"
#include "RandomBoxManager.h"
#include "..\Headers\Inventory.h"
#include "Sound_Manager.h"

USING(Client)

CInventory::CInventory(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < INVEN_END; ++i)
	{
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}

	for (_uint i = 0; i < RANDOMBOX_ITEM_END; ++i)
		m_pTextureRandom[i] = nullptr;
}

CInventory::CInventory(const CInventory & _rOther)
	: CUIObject(_rOther)
{
}

void CInventory::Set_Render(_bool bRender)
{
	m_bRender = bRender;
}

void CInventory::Use_Potion(ePotion_ID ePotionID)
{
	for (auto& pItem : m_pInvenList)
	{
		if (pItem->ePotionID == ePotionID)
		{
			--pItem->iCnt;
			// 다 쓰면 아이템 삭제
			if (pItem->iCnt == 0)
				Delete_Item(pItem->szItemTag);
			return;
		}
	}
}

HRESULT CInventory::Get_ShopItem(const wstring & strItemName)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CDataManager* pItem = (CDataManager*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Item");
	if (nullptr == pItem)
		return E_FAIL;

	

	for (auto& pItem : m_pInvenList)
	{
		// 만약 이미 존재하는 아이템이고
		if (pItem->szItemTag == strItemName)
		{
			// 골드가 있다면 개수만 증가시킨다
			if (m_iGold >= pItem->iPrice)
			{
				++pItem->iCnt;
				m_iGold -= pItem->iPrice;
				//PRINT_LOG(L"상점 아이템 구매 완료", LOG::DEBUG);
				m_bRenderClearWnd = true;
			}
			else
				PRINT_LOG(L"못사", LOG::DEBUG);
			return S_OK;
		}
	}

	_int iPrice = pItem->Get_ItemInfo_Price(strItemName);

	if ((iPrice == -1) || m_iGold < iPrice)
	{
		PRINT_LOG(L"못사", LOG::DEBUG);
	}
	else if (m_iGold >= iPrice)
	{
		m_iGold -= iPrice;

		// 아이템 클래스에서 strItemName(아이템의 이름)으로 구매한 아이템의 텍스처를 가져와서 저장
		if (m_pTextureItem[m_iInsertOrder])
			Safe_Release(m_pTextureItem[m_iInsertOrder]);
		m_pTextureItem[m_iInsertOrder] = pItem->Get_ItemInfo_Texture(strItemName);

		INVEN_ITEM* pInvenItem = new INVEN_ITEM;
		pItem->Get_ItemInfo(strItemName, *pInvenItem);
		pInvenItem->iInvneInsertOrder = m_iInsertOrder;

		m_pInvenList.emplace_back(pInvenItem);

		// 이 칸에는 아이템이 있다고 변경  
		m_bIsItemHere[m_iInsertOrder] = true;
		++m_pInvenList[m_iInsertOrder]->iCnt;

		//PRINT_LOG(L"상점 아이템 구매 완료", LOG::DEBUG);
		m_bRenderClearWnd = true;
		++m_iInsertOrder;
	}

	CSoundManager::Get_Instance()->PlayUI(L"Buy_Item.wav");

	return S_OK;
}

_int CInventory::Get_ItemCount(const wstring & strItemName)
{
	_int iIndex = 0;

	for (auto& pItem : m_pInvenList)
	{
		if (!wcscmp(pItem->szItemTag, strItemName.c_str()))
			return pItem->iCnt;
		++iIndex;
	}

	return 0;
}

HRESULT CInventory::Delete_Item(const wstring & strItemName)
{
	_int iIndex = 0;
	_bool bDeletIndex[36] = { false, };
	auto& iter = m_pInvenList.begin();

	while (iter != m_pInvenList.end())
	{
		if (!wcscmp((*iter)->szItemTag, strItemName.c_str()))
		{
			m_bIsItemHere[iIndex] = false;
			bDeletIndex[iIndex] = true;
			Safe_Delete(*iter);
			iter = m_pInvenList.erase(iter);
			--m_iInsertOrder;
		}
		else
			++iter;

		++iIndex;
	}

	int iTextureItemIdx = 0;
	int iDeleteCnt = 0;
	// 아이템 텍스처 삭제
	for (auto& iter = m_pTextureItem.begin(); iter != m_pTextureItem.end(); )
	{
		if (bDeletIndex[iTextureItemIdx])
		{
			++iDeleteCnt;
			Safe_Release(*iter);
			iter = m_pTextureItem.erase(iter);
		}
		else
			++iter;
		++iTextureItemIdx;
	}

	m_pTextureItem.resize(36);
	// 삭제한만큼 새로 만들어서 넣어준다
	for (_int i = 36 - iDeleteCnt, j = 0; j < iDeleteCnt; i++, ++j)
	{
		// Texture-------------------------------------------------------------------
		TCHAR szItemTexture[MAX_STR] = L"";
		TCHAR szItemTextureName[MAX_STR] = L"";
		//ITEM* pItem = new ITEM;
		//pItem->wstrItemName = L"Empty";
		wsprintf(szItemTextureName, L"Component_Texture_Item_Empty");
		wsprintf(szItemTexture, L"Com_NewItemTexture%d", m_iNewInsertOrder);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC,
			szItemTextureName, szItemTexture, (CComponent**)&m_pTextureItem[i])))
			return E_FAIL;
		++m_iNewInsertOrder;
	}

	for (_uint i = 0; i < m_pInvenList.size(); i++)
	{
		m_bIsItemHere[i] = true;
	}

	return S_OK;
}

HRESULT CInventory::Get_RewardItem(const wstring & strItemName)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CDataManager* pItem = (CDataManager*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Item");
	if (nullptr == pItem)
		return E_FAIL;

	for (auto& pItem : m_pInvenList)
	{
		if (pItem->szItemTag == strItemName)
		{
			++pItem->iCnt;
			return S_OK;
		}
	}

	if (m_pTextureItem[m_iInsertOrder])
		Safe_Release(m_pTextureItem[m_iInsertOrder]);
	m_pTextureItem[m_iInsertOrder] = pItem->Get_ItemInfo_Texture(strItemName);

	INVEN_ITEM* pInvenItem = new INVEN_ITEM;
	pItem->Get_ItemInfo(strItemName, *pInvenItem);
	pInvenItem->iInvneInsertOrder = m_iInsertOrder;

	m_pInvenList.emplace_back(pInvenItem);

	m_bIsItemHere[m_iInsertOrder] = true;
	++m_pInvenList[m_iInsertOrder]->iCnt;
	++m_iInsertOrder;


	return S_OK;
}

HRESULT CInventory::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Setup_GameObject(void * pArg)
{
	m_pTextureItem.resize(36);
	m_pTransformItem.resize(36);

	if (FAILED(Add_Component()))
		return E_FAIL;
	if (FAILED(Add_Component_Item()))
		return E_FAIL;

	m_pTransformCom[INVEN_WND]->Set_Position(vWndPos);
	m_pTransformCom[INVEN_SELL_BUTTON]->Set_Position(_vec3(vWndPos.x + 120.f, vWndPos.y + 250.f, 0.f));
	m_pTransformCom[INVEN_GOLD]->Set_Position(_vec3(vWndPos.x + 25.f, vWndPos.y + 115.f, 0.f));

	_int iIndex = 0;

	for (_uint i = 0; i < 6; ++i)
	{
		for (_uint j = 0; j < 6; ++j)
		{
			iIndex = i * 6 + j;
			m_vItemPos[i][j].x = (j * 60.f) + (vWndPos.x - 167.f);
			m_vItemPos[i][j].y = (i * 60.f) + (vWndPos.y - 170.f);
			m_vItemPos[i][j].z = 0.f;
			m_pTransformItem[iIndex]->Set_Position(m_vItemPos[i][j]);

			//m_tItemCollRt[i][j].left = (LONG)(m_vItemPos[i][j].x - 20.f);
			//m_tItemCollRt[i][j].right = (LONG)(m_vItemPos[i][j].x + 20.f);
			//m_tItemCollRt[i][j].top = (LONG)(m_vItemPos[i][j].y - 20.f);
			//m_tItemCollRt[i][j].bottom = (LONG)(m_vItemPos[i][j].y + 20.f);
		}
	}

	return S_OK;
}

_int CInventory::Update_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;


	// 인벤 on/off
	if ((pManagement->Key_Down('I')))
	{
		CSoundManager::Get_Instance()->PlayUI(L"OpenWnd.wav");
		m_bRender = !m_bRender;
	}

	if (m_bRender)
	{
		//m_bMoveInvenWnd = true;

		// 판매 버튼을 눌렀는지 확인(m_bSelect_SellItem)
		if (FAILED(Check_SellButton()))
			return GAMEOBJECT::WARN;
		// 판매 버튼을 눌렀을 경우
		if (m_bSelect_SellItem)
			// 판매할 아이템 선택
			if (FAILED(Select_SellItem()))
				return GAMEOBJECT::WARN;

		// 자동 정렬 버튼을 눌렀는지 확인
		if (FAILED(Check_AutoSortButton()))
			return GAMEOBJECT::WARN;

		// 아이템 장착
		if (FAILED(Check_EquipItem()))
			return GAMEOBJECT::WARN;

		// 퀵슬롯으로 이동
		if (!m_bSelect_SellItem)
		{
			if (FAILED(Move_To_QuickSlot()))
				return GAMEOBJECT::WARN;
		}

		//// 인벤 창 이동
		//if (!m_bSelect_SellItem && m_bMoveInvenWnd)
		//{
		//	if (FAILED(Move_InventoryWnd()))
		//		return GAMEOBJECT::WARN;
		//}
	}

	if (FAILED(Check_ItemCount()))
		return GAMEOBJECT::ERR;

	for (_uint i = 0; i < INVEN_END; ++i)
		m_pTransformCom[i]->Update_Transform();

	_int iSize = m_pTransformItem.size();
	for (_int i = 0; i < iSize; ++i)
	{
		m_pTransformItem[i]->Update_Transform();
	}

	return GAMEOBJECT::NOEVENT;
}

_int CInventory::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CInventory::Render_UI()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (m_bRender)
	{
		for (_uint i = 0; i < INVEN_END; ++i)
		{
			if (i != INVEN_GOLD)
			{
				const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
				_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
				_vec3 vPos = m_pTransformCom[i]->Get_Desc().vPosition;

				m_tInvenWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
				m_tInvenWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
				m_tInvenWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
				m_tInvenWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

				m_pSprite->SetTransform(&m_pTransformCom[i]->Get_Desc().matWorld);
				m_pSprite->Draw(
					(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
					nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else if (i == INVEN_GOLD)
			{
				TCHAR		szBuff[MAX_PATH] = L"";
				D3DXMATRIX	matScale, matTrans, matWorld;
				StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d", m_iGold);
				
				D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
				D3DXMatrixTranslation(&matTrans, vWndPos.x + 50.f, vWndPos.y + 190.f, 0.f);
				matWorld = matScale * matTrans;

				//m_pSprite->SetTransform(&(matScale * m_pTransformCom[i]->Get_Desc().matWorld));
				m_pSprite->SetTransform(&matWorld);
				m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
					nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}

		if (FAILED(Render_Item()))
			return E_FAIL;
	}

	if (m_bRenderClearWnd)
	{
		if (FAILED(Render_ClearWnd()))
			return E_FAIL;
		if (pManagement->Key_Pressing(VK_RETURN))
			m_bRenderClearWnd = false;
	}

	if (m_bRender_GetRandomBoxItem)
	{
		if (FAILED(Render_GetRandomBoxItem()))
			return E_FAIL;
		if (pManagement->Key_Pressing(VK_RETURN))
			m_bRender_GetRandomBoxItem = false;
	}

	return S_OK;
}

HRESULT CInventory::Check_ItemCount()
{
	auto& iter = m_pInvenList.begin();
	_int iDeleteCnt = 0, iIndex = 0;
	_bool bDelete[36] = { false, };

	while (iter != m_pInvenList.end())
	{
		// 아이템 개수가 0이면 아이템/텍스처 삭제
		if ((*iter)->iCnt == 0)
		{
			++iDeleteCnt;
			bDelete[iIndex] = true;
			Safe_Delete(*iter);
			iter = m_pInvenList.erase(iter);
			m_bIsItemHere[iIndex] = false;
		}
		else
			++iter;
		++iIndex;
	}

	// 아이템 텍스처 삭제
	_int iTextureIndex = 0;
	for (auto& iter = m_pTextureItem.begin(); iter != m_pTextureItem.end(); )
	{
		if (bDelete[iTextureIndex])
		{
			Safe_Release(*iter);
			iter = m_pTextureItem.erase(iter);
		}
		else
			++iter;
		++iTextureIndex;
	}

	// 삭제한 텍스처만큼 생성
	m_pTextureItem.resize(36);
	for (_int i = 36 - iDeleteCnt, j = 0; j < iDeleteCnt; i++, ++j)
	{
		// Texture-------------------------------------------------------------------
		TCHAR szItemTexture[MAX_STR] = L"";
		TCHAR szItemTextureName[MAX_STR] = L"";
		//ITEM* pItem = new ITEM;
		//pItem->wstrItemName = L"Empty";
		wsprintf(szItemTextureName, L"Component_Texture_Item_Empty");
		wsprintf(szItemTexture, L"Com_NewItemTexture%d", m_iNewInsertOrder);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC,
			szItemTextureName, szItemTexture, (CComponent**)&m_pTextureItem[i])))
			return E_FAIL;
		++m_iNewInsertOrder;
	}

	return S_OK;
}

HRESULT CInventory::Check_SellButton()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (pManagement->Key_Pressing(VK_LBUTTON) && !m_bMovingClear)
	{
		CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
		if (nullptr == pMouse)
			return E_FAIL;

		if (PtInRect(&m_tInvenWndCollRt[INVEN_SELL_BUTTON], pMouse->Get_Point()))
		{
			if (!m_bSelect_SellItem)
			{
				m_bSelect_SellItem = true;
				PRINT_LOG(L"판매 모드로 변경", LOG::DEBUG);
			}
			else if (m_bSelect_SellItem)
			{
				// 판매할 아이템 선택 끝
				m_bSelect_SellItem = false;
				// 이제 선택된 아이템들은 빈 공간으로 그려준다
				m_bRenderEmptySell = true;

				m_iGold += m_iSellGold;
				m_iSellGold = 0;

				m_iInsertOrder -= m_iDeleteCnt;
				m_iDeleteCnt = 0;

				// 이 때, 퀵슬롯에 있는 포션 아이템들은 바로 삭제되어야 한다
				if (FAILED(Change_PotionCnt()))
					return E_FAIL;

				PRINT_LOG(L"아이템 판매 시작.", LOG::DEBUG);

				CSoundManager::Get_Instance()->PlayUI(L"Sell_Item.wav");
			}
		}
	}
	return S_OK;
}

HRESULT CInventory::Select_SellItem()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return E_FAIL;

	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return E_FAIL;

	int iIndex = 0;

	for (_uint i = 0; i < 6; i++)
	{
		for (_uint j = 0; j < 6; j++)
		{
			if (pManagement->Key_Pressing(VK_LBUTTON))
			{
				iIndex = i * 6 + j;
		
				if (PtInRect(&m_tItemCollRt[i][j], pMouse->Get_Point()))
				{
					_int k = 0;
					// 아이템이 있는 칸들만 선택 할 수 있음
					if (m_bIsItemHere[iIndex] && !m_bSelectedSell[iIndex])
					{
						// 장착하고 있으면 판매할 수 없음
						if (!pEquip->Check_IsItemEquip(m_pInvenList[iIndex]->szItemTag))
						{
							m_bSelectedSell[iIndex] = true;
							++m_iDeleteCnt;
							m_iSellGold += (m_pInvenList[iIndex]->iPrice * m_pInvenList[iIndex]->iCnt);
							CSoundManager::Get_Instance()->PlayUI(L"OpenWnd.wav");
						}
						else
							PRINT_LOG(L"현재 장착하고 있는 아이템은 판매 못함", LOG::DEBUG);
					}
					else if (!m_bIsItemHere[iIndex])
					{
						continue;
					}
				}
			}
		}
	}
	return S_OK;
}

HRESULT CInventory::Check_AutoSortButton()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	int iItemListSize = m_pInvenList.size();

	// 지금은 키입력으로 하는데 나중에 버튼 추가해야 함
	if (pManagement->Key_Down('M'))
	{
		auto& iter = m_pInvenList.begin();
		_uint iSize = m_pInvenList.size();
		int iItemIndex = 0;
		m_bAutoSort = true;

		// 인벤에서 아이템 삭제
		while (iter != m_pInvenList.end())
		{
			if (m_bSelectedSell[iItemIndex])
			{
				//m_bIsItemHere[iItemIndex] = false;
				Safe_Delete(*iter);
				iter = m_pInvenList.erase(iter);
			}
			else
				++iter;

			++iItemIndex;
		}

		int iTextureItemIdx = 0;
		int iDeleteCnt = 0;
		// 아이템 텍스처 삭제
		for (auto& iter = m_pTextureItem.begin(); iter != m_pTextureItem.end(); )
		{
			if (m_bSelectedSell[iTextureItemIdx])
			{
				++iDeleteCnt;
				Safe_Release(*iter);
				iter = m_pTextureItem.erase(iter);
				m_bSelectedSell[iTextureItemIdx] = false;
			}
			else
				++iter;
			++iTextureItemIdx;
		}
		_int m = 0;
		m_pTextureItem.resize(36);
		// 삭제한만큼 새로 만들어서 넣어준다
		for (_int i = 36 - iDeleteCnt, j = 0; j < iDeleteCnt; i++, ++j)
		{
			// Texture-------------------------------------------------------------------
			TCHAR szItemTexture[MAX_STR] = L"";
			TCHAR szItemTextureName[MAX_STR] = L"";
			//ITEM* pItem = new ITEM;
			//pItem->wstrItemName = L"Empty";
			wsprintf(szItemTextureName, L"Component_Texture_Item_Empty");
			wsprintf(szItemTexture, L"Com_NewItemTexture%d", m_iNewInsertOrder);

			if (FAILED(CGameObject::Add_Component(SCENE_STATIC, 
				szItemTextureName, szItemTexture, (CComponent**)&m_pTextureItem[i])))
				return E_FAIL;
			++m_iNewInsertOrder;
		}

		for (_int i = 0; i < iDeleteCnt + 1; ++i)
		{
			m_bIsItemHere[i] = true;
		}

		for (_int i = iDeleteCnt + 1; i < 36; ++i)
		{
			m_bIsItemHere[i] = false;
		}

		// 정렬 끝
		//m_bAutoSort = false;
		m_bRenderEmptySell = false;
	}

	return S_OK;
}

HRESULT CInventory::Check_EquipItem()
{
	int iIndex = 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 1);

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return E_FAIL;

	for (_uint i = 0; i < 6; i++)
	{
		for (_uint j = 0; j < 6; j++)
		{
			if (pManagement->Key_Pressing(VK_RBUTTON))
			{
				iIndex = i * 6 + j;
				if (m_bIsItemHere[iIndex] && PtInRect(&m_tItemCollRt[i][j], pMouse->Get_Point()) && 
					m_pInvenList[iIndex]->eSort != RANDOM_POTION && m_pInvenList[iIndex]->eSort != RANDOM_EQUIP)
				{
					_int k = 0;
					// 아이템이 있는 칸들만 선택 할 수 있음
					if (m_bIsItemHere[iIndex])
					{
						// 장비창에게 아이템 정보를 넘겨준다
						//pEquip->Equip_Item(*m_pInvenList[iIndex]);
						pEquip->Equip_Item(m_pInvenList[iIndex]->eSort, m_pInvenList[iIndex]->szItemTag);
						CSoundManager::Get_Instance()->PlayUI(L"Equip_Item.wav");
						return S_OK;
					}
				}
			}
			else if (pManagement->Key_Pressing(VK_LBUTTON) && m_bIsItemHere[iIndex])
			{
				iIndex = i * 6 + j;
				if (PtInRect(&m_tItemCollRt[i][j], pMouse->Get_Point()))
				{
					if (m_pInvenList[iIndex]->eSort == RANDOM_POTION || m_pInvenList[iIndex]->eSort == RANDOM_EQUIP)
					{
						if (FAILED(Open_RandomBox(m_pInvenList[iIndex]->eSort, m_pInvenList[iIndex]->szItemTag)))
							return E_FAIL;
					}
				}
			}
		}
	}
	return S_OK;
}

HRESULT CInventory::Open_RandomBox(eITEM_SORT eSort, const wstring& strNameTag)
{
	wstring strGetcha;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	if (eSort == RANDOM_POTION)
		CRandomBoxManager::Get_Instance()->Gatcha_PotionBox(strGetcha);
	else if (eSort == RANDOM_EQUIP)
		CRandomBoxManager::Get_Instance()->Gatcha_EquipBox(strGetcha);

	if (pManagement != nullptr)
	{
		if (strGetcha == L"Component_Texture_Item_RedPotion")
		{
			m_eRandomBoxID = POTION_RANDOM_REDPOTION;
			strGetcha = L"RedPotion";
		}
		else if (strGetcha == L"Component_Texture_Item_BluePotion")
		{
			m_eRandomBoxID = POTION_RANDOM_BLUEPOTION;
			strGetcha = L"BluePotion";
		}
		else if (strGetcha == L"Component_Texture_Item_RedElixir")
		{
			m_eRandomBoxID = POTION_RANDOM_REDELIXER;
			strGetcha = L"RedElixir";
		}
		else if (strGetcha == L"Component_Texture_Item_BlueElixir")
		{
			m_eRandomBoxID = POTION_RANDOM_BLUEELIXER;
			strGetcha = L"BlueElixir";
		}

		if (strGetcha == L"Component_Texture_Item_WarriorStaff")
		{
			m_eRandomBoxID = EQUIP_RANDOM_STAFF;
			strGetcha = L"Warrior_Staff";
		}
		else if (strGetcha == L"Component_Texture_Item_AquaGloves")
		{
			m_eRandomBoxID = EQUIP_RANDOM_GLOVES;
			strGetcha = L"AquaGloves";
		}
		else if (strGetcha == L"Component_Texture_Item_PupleDress")
		{
			m_eRandomBoxID = EQUIP_RANDOM_DRESS;
			strGetcha = L"PupleDress";
		}
		else if (strGetcha == L"Component_Texture_Item_BalrogWings")
		{
			m_eRandomBoxID = EQUIP_RANDOM_WING;
			strGetcha = L"BalrogWings";
		}

		Delete_Item(strNameTag);
		Get_RewardItem(strGetcha);
		
		// 랜덤박스 연출
		m_bRender_GetRandomBoxItem = true;
	}
	return S_OK;
}


HRESULT CInventory::Move_To_QuickSlot()
{
	_int iIndex = 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CMainUI* pMainUI = (CMainUI*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 0);

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return E_FAIL;

	for (_uint i = 0; i < 6; i++)
	{
		for (_uint j = 0; j < 6; j++)
		{
			if (pManagement->Key_Pressing(VK_LBUTTON))
			{
				iIndex = i * 6 + j;
			
				if (PtInRect(&m_tItemCollRt[i][j], pMouse->Get_Point()) && !m_bMovingClear)
				{
					_int k = 0;
					m_bMoveInvenWnd = false;
					// 아이템이 있는 칸들만 선택 할 수 있음
					if (m_bIsItemHere[iIndex])
					{
						pMainUI->Get_QuickSlotItem(m_pInvenList[iIndex]);
						return S_OK;
					}

				}
				else if (!m_bIsItemHere[iIndex])
					continue;
			}
		}
	}

	return S_OK;
}

HRESULT CInventory::Change_PotionCnt()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CMainUI* pMain = (CMainUI*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 0);
	if (pMain == nullptr)
		return E_FAIL;

	auto& iter = m_pInvenList.begin();
	_int iIndex = 0;

	while (iter != m_pInvenList.end())
	{
		if (m_bSelectedSell[iIndex])
		{
			if ((*iter)->eSort == POTION)
			{
				// 포션 종류가 삭제되었으면 퀵슬롯에게 바로 알려준다
				pMain->Delete_Potion(*iter);
			}
		}

		++iter;
		++iIndex;
	}
	return S_OK;
}

HRESULT CInventory::Render_Item()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	size_t iSize = m_pInvenList.size();
	for (_uint i = 0; i < 6; ++i)
	{
		for (_uint j = 0; j < 6; ++j)
		{
			_bool bRenderCnt = true;
			_int iIndex = i * 6 + j;


			// 정렬할 때 삭제한 벡터만큼 새로 생성해줘야 함
			// -> 근데 아직 구현안해서 그냥 막아놓음
			if (iIndex >= (_int)iSize)
				return S_OK;

			const D3DXIMAGE_INFO* pTexInfo = m_pTextureItem[iIndex]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

			_vec3 vPos = m_pTransformItem[iIndex]->Get_Desc().vPosition;

			m_tItemCollRt[i][j].left = (LONG)(vPos.x - vCenter.x);
			m_tItemCollRt[i][j].right = (LONG)(vPos.x + vCenter.x);
			m_tItemCollRt[i][j].top = (LONG)(vPos.y - vCenter.y);
			m_tItemCollRt[i][j].bottom = (LONG)(vPos.y + vCenter.y);

			// 판매 아이템 선택중일 때===================================================================================================
			if (!m_bRenderEmptySell)
			{
				// 판매 아이템으로 선택되었다면
				if (m_bSelectedSell[iIndex])
				{
					bRenderCnt = true;
					// 선택되었다는 표시를 그리고
					const D3DXIMAGE_INFO* pTexInfo = m_pTextureSell->Get_TexInfo(0);
					_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
					_vec3 vPos = m_pTransformItem[iIndex]->Get_Desc().vPosition;

					// 변경
					D3DXMatrixScaling(&matScale, 1.9f, 1.9f, 0.f);
					D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
					matWorld = matScale * matTrans;

					m_pSprite->SetTransform(&matWorld);
					m_pSprite->Draw(
						(LPDIRECT3DTEXTURE9)m_pTextureSell->GetTexture(0),
						nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				// 아이템을 그린다
				const D3DXIMAGE_INFO* pTexInfo = m_pTextureItem[iIndex]->Get_TexInfo(0);
				_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
				_vec3 vPos = m_pTransformItem[iIndex]->Get_Desc().vPosition;

				// 변경
				D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
				D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
				matWorld = matScale * matTrans;

				m_pSprite->SetTransform(&/*m_pTransformItem[iIndex]->Get_Desc().*/matWorld);
				m_pSprite->Draw(
					(LPDIRECT3DTEXTURE9)m_pTextureItem[iIndex]->GetTexture(0),
					nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			//===========================================================================================================================


			// 판매 아이템들을 선택한 후, 판매 버튼을 눌렀다면=================================================================================
			else if (m_bRenderEmptySell)
			{
				// 판매 아이템으로 선택된 아이템들은 빈 텍스처로 그린다
				if (m_bSelectedSell[iIndex])
				{
					bRenderCnt = false;
					const D3DXIMAGE_INFO* pTexInfo = m_pTextureEmpty->Get_TexInfo(0);
					_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

					m_pSprite->SetTransform(&m_pTransformItem[iIndex]->Get_Desc().matWorld);
					m_pSprite->Draw(
						(LPDIRECT3DTEXTURE9)m_pTextureEmpty->GetTexture(0),
						nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				// 판매 아이템으로 선택되지 않은 아이템들은 그대로 그린다
				else
				{
					_vec3 vPos = m_pTransformItem[iIndex]->Get_Desc().vPosition;

					// 변경
					D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
					D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
					matWorld = matScale * matTrans;

					m_pSprite->SetTransform(&/*m_pTransformItem[iIndex]->Get_Desc().*/matWorld);
					m_pSprite->Draw(
						(LPDIRECT3DTEXTURE9)m_pTextureItem[iIndex]->GetTexture(0),
						nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

				}
				//===========================================================================================================================
			}

			// 아이템 개수
			if (/*!m_bSelectedSell[iIndex] && */bRenderCnt)
			{
				TCHAR		szBuff[MAX_PATH] = L"";
				D3DXMATRIX	matScale, matTrans, matWorld;
				D3DXMatrixIdentity(&matWorld);
				StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d", m_pInvenList[iIndex]->iCnt);

				// 변경
				D3DXMatrixTranslation(&matTrans, vPos.x + 20.f, vPos.y + 20.f, 0.f);
				D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
				matWorld = matScale * matTrans;

				m_pSprite->SetTransform(&matWorld);
				m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
					nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}

	return S_OK;
}

HRESULT CInventory::Render_ClearWnd()
{
	_matrix matWorld, matTrans;
	const D3DXIMAGE_INFO* pTexInfo = m_pTextureClear->Get_TexInfo(0);
	_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
	matWorld = matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pSprite->Draw(
		(LPDIRECT3DTEXTURE9)m_pTextureClear->GetTexture(0),
		nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	return S_OK;
}

HRESULT CInventory::Render_GetRandomBoxItem()
{
	_matrix matWorld, matTrans;
	const D3DXIMAGE_INFO* pTexInfo = m_pTextureRandom[m_eRandomBoxID]->Get_TexInfo(0);
	_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
	matWorld = matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pSprite->Draw(
		(LPDIRECT3DTEXTURE9)m_pTextureRandom[m_eRandomBoxID]->GetTexture(0),
		nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	return S_OK;
}

HRESULT CInventory::Move_InventoryWnd()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return E_FAIL;

	_int iIndex = 0;

	// 아이템 가리키고 있을 땐 인벤창 움직이면 안됨
	for (_uint i = 0; i < 6; i++)
	{
		for (_uint j = 0; j < 6; j++)
		{
			
			iIndex = i * 6 + j;
		
			if (PtInRect(&m_tItemCollRt[i][j], pMouse->Get_Point()))
			{
				m_bMoveInvenWnd = false;
				return S_OK;
			}
			
		}
	}

	if (pManagement->Key_Pressing(VK_LBUTTON))
	{
		if (PtInRect(&m_tInvenWndCollRt[INVEN_WND], pMouse->Get_Point()))
		{
			vWndPos.x = (_float)pMouse->Get_Point().x;
			vWndPos.y = (_float)pMouse->Get_Point().y;
			m_pTransformCom[INVEN_WND]->Set_Position(vWndPos);
			m_pTransformCom[INVEN_SELL_BUTTON]->Set_Position(_vec3(vWndPos.x + 120.f, vWndPos.y + 250.f, 0.f));
			m_pTransformCom[INVEN_GOLD]->Set_Position(_vec3(vWndPos.x + 25.f, vWndPos.y + 115.f, 0.f));


			if (Change_AllPos())
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CInventory::Change_AllPos()
{
	D3DXMATRIX matWorld;
	matWorld = m_pTransformCom[INVEN_WND]->Get_Desc().matWorld;

	//for (_uint i = 1; i < INVEN_END; ++i)
	//{
	//	m_pTransformCom[i]->Set_WorldMatrix(
	//		m_pTransformCom[i]->Get_Desc().matWorld * matWorld);
	//}

	//m_pTransformCom[INVEN_SELL_BUTTON]->Set_Position(_vec3(vWndPos.x + 110.f, vWndPos.y + 180.f, 0.f));
	//m_pTransformCom[INVEN_GOLD]->Set_Position(_vec3(525.f, 335.f, 0.f));
	//m_pTransformCom[INVEN_GOLD]->Set_Position(_vec3(vWndPos.x + 25.f, vWndPos.y + 115.f, 0.f));

	_int iIndex = 0;
	for (_uint i = 0; i < 6; ++i)
	{
		for (_uint j = 0; j < 6; ++j)
		{
			iIndex = i * 6 + j;
			//m_vItemPos[i][j].x = (j * 45.f) + 380.f;
			//m_vItemPos[i][j].y = (i * 30.f) + 85.f;
			m_vItemPos[i][j].x = (j * 45.f) + (vWndPos.x - 120.f);
			m_vItemPos[i][j].y = (i * 30.f) + (vWndPos.y - 115.f);
			m_vItemPos[i][j].z = 0.f;
			m_pTransformItem[iIndex]->Set_Position(m_vItemPos[i][j]);
		}
	}

	return S_OK;
}

HRESULT CInventory::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Texture_Shop_ClearWnd",
		L"Com_TextureClear", (CComponent**)&m_pTextureClear)))
		return E_FAIL;

	TCHAR szTextureTag[][MAX_STR] =
	{
		L"Component_Texture_GetStaff",
		L"Component_Texture_GetGloves",
		L"Component_Texture_GetDress",
		L"Component_Texture_GetWing",

		L"Component_Texture_GetRedPotion",
		L"Component_Texture_GetBluePotion",
		L"Component_Texture_GetRedElixer",
		L"Component_Texture_GetBlueElixer"
	};
	TCHAR szTexture2[MIN_STR] = L"Com_TextureGetRandomItem%d";
	TCHAR szCombine2[MIN_STR] = L"";

	for (_uint i = 0; i < RANDOMBOX_ITEM_END; i++)
	{
		StringCchPrintf(szCombine2, _countof(szCombine2), szTexture2, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureTag[i],
			szCombine2, (CComponent**)&m_pTextureRandom[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < INVEN_END; ++i)
	{
		// 2. Transform
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, _countof(szTransform),
			L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;

		if (i == INVEN_GOLD)
			return S_OK;
		// 3. Texture
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		if (i == INVEN_WND)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Inven_InvenWnd");
		if (i == INVEN_SELL_BUTTON)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Inven_SellButton");
		StringCchPrintf(szTexture, _countof(szTexture),
				L"Com_Texturem%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	//// 4. ItemMgr
	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, 
	//	L"Component_ItemManagement", L"Com_ItemMgr", 
	//	(CComponent**)&m_pItemMgrCom)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CInventory::Add_Component_Item()
{
	for (_uint i = 0; i < 36; ++i)
	{
		// Texture-------------------------------------------------------------------
		TCHAR szItemTexture[MAX_PATH] = L"";
		TCHAR szItemTextureName[MAX_PATH] = L"";
		StringCchPrintf(szItemTexture, _countof(szItemTexture),
			L"Com_ItemTexture%d", i);
		StringCchPrintf(szItemTextureName, _countof(szItemTextureName),
			L"Component_Texture_Item_Empty");

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szItemTextureName,
			szItemTexture, (CComponent**)&m_pTextureItem[i])))
			return E_FAIL;

		// Transform-------------------------------------------------------------------
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, _countof(szTransform),
			L"Com_ItemTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformItem[i])))
			return E_FAIL;

	}

	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Texture_Select_SellItem",
		L"Com_SellItemTexture", (CComponent**)&m_pTextureSell)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Texture_Item_Empty",
		L"Com_EmptyItemTexture", (CComponent**)&m_pTextureEmpty)))
		return E_FAIL;

	return S_OK;
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CInventory* pInstance = new CInventory(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CInven", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInventory::Clone_GameObject(void * pArg)
{
	CInventory* pInstance = new CInventory(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CInven", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	//Safe_Release(m_pItemMgrCom);
	Safe_Release(m_pTextureSell);
	Safe_Release(m_pTextureEmpty);

	for (_uint i = 0; i < INVEN_END; i++)
	{
		Safe_Release(m_pTransformCom[i]);
		if (i == INVEN_GOLD)
			break;
		Safe_Release(m_pTextureCom[i]);
	}

	for (auto& pItem : m_pTextureItem)
		Safe_Release(pItem);
	m_pTextureItem.clear();
	
	for (auto& pItem : m_pTransformItem)
		Safe_Release(pItem);
	m_pTransformItem.clear();
	//m_pTransformItem.swap(m_pTransformItem);


	for (auto& pItem : m_pInvenList)
		Safe_Delete(pItem);
	m_pInvenList.clear();
	//m_pInvenList.swap(m_pInvenList);

	Safe_Release(m_pTextureClear);

	for (_uint i = 0; i < RANDOMBOX_ITEM_END; i++)
		Safe_Release(m_pTextureRandom[i]);

	CUIObject::Free();
}
