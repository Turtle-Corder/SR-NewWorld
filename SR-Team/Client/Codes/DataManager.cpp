#include "stdafx.h"
#include "Status.h"
#include "UICamera.h"
#include "Inventory.h"
#include "..\Headers\DataManager.h"

USING(Client)


CDataManager::CDataManager(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint i = 0; i < 30; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pStatCom[i] = nullptr;
	}
	for (_uint i = 0; i < 11; i++)
		m_pTextureSkillIcon[i] = nullptr;
}

CDataManager::CDataManager(const CDataManager & _rOther)
	: CGameObject(_rOther)
{
}

CTexture * CDataManager::Get_ItemInfo_Texture(const wstring & strItemTag)
{
	_int iIndex = 0;
	_int iSkillIndex = 0;

	for (auto& pItem : m_vItemList)
	{
		if (!wcscmp(pItem->szItemTag, strItemTag.c_str()))
			return m_pTextureCom[iIndex];
		++iIndex;
	}

	for (auto& pItem : m_vSkillIconList)
	{
		if (!wcscmp(pItem->szItemTag, strItemTag.c_str()))
			return m_pTextureSkillIcon[iSkillIndex];
		++iSkillIndex;
	}

	return nullptr;
}

const _int CDataManager::Get_ItemInfo_Price(const wstring & strItemTag)
{
	_int iIndex = 0;
	for (auto& pItem : m_vItemList)
	{
		if (!wcscmp(pItem->szItemTag, strItemTag.c_str()))
			return pItem->iPrice;
		++iIndex;
	}

	return -1;
}

HRESULT CDataManager::Get_ItemInfo(const wstring & strItemTag, INVEN_ITEM & tItem)
{
	_int iIndex = 0;
	for (auto& pItem : m_vItemList)
	{
		if (!wcscmp(pItem->szItemTag, strItemTag.c_str()))
		{
			memcpy_s(&tItem, sizeof(INVEN_ITEM), pItem, sizeof(INVEN_ITEM));
			return S_OK;
		}
		++iIndex;
	}

	return E_FAIL;
}



CStatus * CDataManager::Get_ItemStat(const wstring & strItemTag)
{
	_int iIndex = 0;
	for (auto& pItem : m_vItemList)
	{
		if (!wcscmp(pItem->szItemTag, strItemTag.c_str()))
			return m_pStatCom[iIndex];
		++iIndex;
	}

	return nullptr;
}

INVEN_ITEM* CDataManager::Get_ActiveSkillIcon(_int iSkillID)
{
	_int iIndex = 0;
	for (auto& pItem : m_vSkillIconList)
	{
		if (pItem->eActiveID == iSkillID)
		{
			//memcpy_s(&tSkill, sizeof(INVEN_ITEM), pItem, sizeof(INVEN_ITEM));
			return pItem;
		}
		++iIndex;
	}

	return nullptr;
}

HRESULT CDataManager::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CDataManager::Setup_GameObject(void * pArg)
{
	if (Add_Component())
		return E_FAIL;
	if (Add_Component_Item())
		return E_FAIL;
	if (Add_Component_SkillIcon())
		return E_FAIL;

	return S_OK;
}

int CDataManager::Update_GameObject(float DeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

int CDataManager::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

HRESULT CDataManager::Render_UI()
{
	return S_OK;
}

HRESULT CDataManager::Add_Component()
{
	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_ItemManagement", L"Com_ItemMgr", (CComponent**)&m_pItmeMgrCom)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CDataManager::Add_Component_Item()
{
	for (_uint i = 0; i < 30; ++i)
	{
		// 3. Texture--------------------------------------------------------------
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		if (i == 0)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_MagicalRageStaff");
		else if (i == 1)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_AngleWingStaff");
		else if (i == 2)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_WarriorStaff");
		else if (i == 3)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_BlackDress");
		else if (i == 4)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_PupleDress");
		else if (i == 5)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_ScholarShoes");
		else if (i == 6)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_ArcaneShoes");
		else if (i == 7)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_RedPotion");
		else if (i == 8)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_OrangePotion");
		else if (i == 9)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_WhitePotion");
		else if (i == 10)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_BluePotion");
		else if (i == 11)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_RedElixir");
		else if (i == 12)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_BlueElixir");
		else if (i == 13)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_AbsoluteBelt");
		else if (i == 14)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_AquaGloves");
		else if (i == 15)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_BalrogWings");
		else if (i == 16)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_SorcererGloves");
		else if (i == 17)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_FlowerQuest_Flower");
		else if (i == 18)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_FlowerQuest_RewardPotion");
		else if (i == 19)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainQuest_HelpWnd_GolemCore_Red");
		else if (i == 20)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainQuest_HelpWnd_GolemCore_Green");
		else if (i == 21)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainQuest_HelpWnd_GolemCore_Puple");
		else if (i == 22)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainQuest_HelpWnd_GolemCore_Blue");
		else if (i == 23)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_DropDiamond");
		else if (i == 24)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_DropRuby");
		else if (i == 25)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_MagicalStaff");
		else if (i == 26)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_ItemBox_Potion");
		else if (i == 27)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_ItemBox_Equip");
		else if (i == 28)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_ForgottenNecklace");
		else if (i == 29)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Item_LethalCape");

		StringCchPrintf(szTexture, _countof(szTexture),
			L"Com_Texture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;


		// 아이템 정보----------------------------------------------------------------
		INVEN_ITEM* pItem = new INVEN_ITEM;
		if (i == 0)
		{
			pItem->eSort = eITEM_SORT::STAFF1;
			pItem->iPrice = 200;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"Magical_Rate_Staff");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"분노의 지팡이");
		}
		if (i == 1)
		{
			pItem->eSort = eITEM_SORT::STAFF1;
			pItem->iPrice = 100;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"Angle_Wing_Staff");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"천사의 지팡이");
		}
		if (i == 2)
		{
			pItem->iPrice = 300;
			pItem->eSort = eITEM_SORT::STAFF1;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"Warrior_Staff");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"강한 지팡이");
		}
		if (i == 3)
		{
			pItem->iPrice = 150;
			pItem->eSort = eITEM_SORT::SUIT;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BlackDress");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"양복");
		}
		if (i == 4)
		{
			pItem->iPrice = 200;
			pItem->eSort = eITEM_SORT::SUIT;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"PupleDress");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"초급 마법사 옷");
		}
		if (i == 5)
		{
			pItem->iPrice = 130;
			pItem->eSort = eITEM_SORT::SHOES;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"ScholarShoes");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"학자의 신발");
		}
		if (i == 6)
		{
			pItem->iPrice = 270;
			pItem->eSort = eITEM_SORT::SHOES;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"ArcaneShoes");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"신비한 신발");
		}
		if (i == 7)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::POTION;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"RedPotion");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"빨간 포션");
			pItem->ePotionID = RED_POTION;
		}
		if (i == 8)
		{
			pItem->iPrice = 150;
			pItem->eSort = eITEM_SORT::POTION;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"OrangePotion");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"주황 포션");
		}
		if (i == 9)
		{
			pItem->iPrice = 200;
			pItem->eSort = eITEM_SORT::POTION;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"WhitePotion");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"하얀 포션");
		}
		if (i == 10)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::POTION;
			pItem->ePotionID = ePotion_ID::BLUE_POTION;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BluePotion");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"파란 포션");
		}
		if (i == 11)
		{
			pItem->iPrice = 200;
			pItem->eSort = eITEM_SORT::POTION;
			pItem->ePotionID = ePotion_ID::RED_ELIXIR;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"RedElixir");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"빨간 엘릭서");
		}
		if (i == 12)
		{
			pItem->iPrice = 200;
			pItem->eSort = eITEM_SORT::POTION;
			pItem->ePotionID = ePotion_ID::BLUE_ELIXIR;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BlueElixir");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"파란 엘릭서");
		}
		if (i == 13)
		{
			pItem->iPrice = 400;
			pItem->eSort = eITEM_SORT::NECKLACE2;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"AbsoluteBelt");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"순수한 벨트");
		}
		if (i == 14)
		{
			pItem->iPrice = 350;
			pItem->eSort = eITEM_SORT::GLOVES;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"AquaGloves");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"아쿠아 장갑");
		}
		if (i == 15)
		{
			pItem->iPrice = 500;
			pItem->eSort = eITEM_SORT::CLOAK;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BalrogWings");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"발록의 날개");
		}
		if (i == 16)
		{
			pItem->iPrice = 180;
			pItem->eSort = eITEM_SORT::GLOVES;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"SorcererGloves");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"마법사장갑");
		}
		if (i == 17)
		{
			pItem->iPrice = 50;
			pItem->eSort = eITEM_SORT::QUEST1;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"flower");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"특별한 꽃");
		}
		if (i == 18)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::POTION;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"RewardPotion");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"포션");
		}

		if (i == 19)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::MAIN_QUEST;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"GolemCore_Red");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"빨간 골렘의 핵");
		}
		if (i == 20)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::MAIN_QUEST;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"GolemCore_Green");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"초록 골렘의 핵");
		}
		if (i == 21)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::MAIN_QUEST;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"GolemCore_Puple");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"보라색 골렘의 핵");
		}
		if (i == 22)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::MAIN_QUEST;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"GolemCore_Blue");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"파란 골렘의 핵");
		}
		if (i == 23)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::DROP;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"Diamond");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"다이아몬드");
		}
		if (i == 24)
		{
			pItem->iPrice = 100;
			pItem->eSort = eITEM_SORT::DROP;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"Ruby");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"루비 보석");
		}
		if (i == 25)
		{
			pItem->iPrice = 1000;
			pItem->eSort = eITEM_SORT::STAFF1;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"MagicalStaff");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"마법 스태프");
		}
		if (i == 26)
		{
			pItem->iPrice = 500;
			pItem->eSort = eITEM_SORT::RANDOM_POTION;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"Potion_Box");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"물약 상자");
		}
		if (i == 27)
		{
			pItem->iPrice = 500;
			pItem->eSort = eITEM_SORT::RANDOM_EQUIP;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"Equip_Box");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"장비 상자");
		}
		if (i == 28)
		{
			pItem->iPrice = 160;
			pItem->eSort = eITEM_SORT::NECKLACE1;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"ForgottenNecklace");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"잊혀진 목걸이");
		}
		if (i == 29)
		{
			pItem->iPrice = 300;
			pItem->eSort = eITEM_SORT::CLOAK;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"LethalCape");
			StringCchPrintf(pItem->szShopTag, _countof(pItem->szShopTag),
				L"영웅의 망토");
		}
		m_vItemList.emplace_back(pItem);


		// Stat-----------------------------------------------------------------------
		CStatus::STAT	tStat;
		if (i == 0)
		{
			tStat.iCriticalRate = 20;
			tStat.iMaxAtt = 150;
			tStat.iMinAtt = 10;
		}
		else if (i == 1)
		{
			tStat.iCriticalRate = 20;
			tStat.iMaxAtt = 160;
			tStat.iMinAtt = 50;
		}
		else if (i == 2)
		{
			tStat.iCriticalRate = 30;
			tStat.iMaxAtt = 120;
			tStat.iMinAtt = 70;
		}
		else if (i == 3)
		{
			tStat.iDef = 500;
		}
		else if (i == 4)
			tStat.iDef = 300;
		else if (i == 5)
			tStat.iDef = 200;
		else if (i == 6)
			tStat.iDef = 100;
		else if (i == 7)
			tStat.iHp = 50;
		else if (i == 8)
			tStat.iHp = 100;
		else if (i == 9)
			tStat.iHp = 150;
		else if (i == 10)
			tStat.iMp = 50;
		else if (i == 11)
			tStat.iHp = 100;
		else if (i == 12)
			tStat.iMp = 100;
		else if (i == 13)
		{
			tStat.iCriticalChance = 20;
		}
		else if (i == 14)
		{
			tStat.iCriticalRate = 30;
		}
		else if (i == 15)
		{
			tStat.iMaxAtt = 100;
			tStat.iCriticalRate = 50;
		}
		else if (i == 16)
		{
			tStat.iCriticalChance = 20;
		}
		else if (i == 17)
		{
			tStat.iHp = 50;
			tStat.iMp = 50;
		}
		else if (i == 18)
		{
			tStat.iHp = 100;
			tStat.iMp = 100;
		}

		else if (i == 19)
		{
			tStat.iHp = 100;
			tStat.iMp = 100;
		}
		else if (i == 20)
		{
			tStat.iHp = 100;
			tStat.iMp = 100;
		}
		else if (i == 21)
		{
			tStat.iHp = 100;
			tStat.iMp = 100;
		}
		else if (i == 22)
		{
			tStat.iHp = 100;
			tStat.iMp = 100;
		}
		else if (i == 23)
		{
		}
		else if (i == 24)
		{
		}
		else if (i == 25)
		{
			tStat.iMaxAtt = 500;
			tStat.iMinAtt = 50;
			tStat.iCriticalRate = 60;
		}
		else if (i == 26)
		{
		}
		else if (i == 27)
		{
		}

		else if (i == 28)
		{
			tStat.iCriticalRate = 30;
		}
		else if (i == 29)
		{
			tStat.iMaxAtt = 100;
		}

		TCHAR szStat[MAX_PATH] = L"";
		StringCchPrintf(szStat, _countof(szStat),
			L"Com_Stat%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Status",
			szStat, (CComponent**)&m_pStatCom[i], &tStat)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDataManager::Add_Component_SkillIcon()
{
	for (_uint i = 0; i < 11; ++i)
	{
		// 3. Texture--------------------------------------------------------------
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		if (i == 0)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_ElementalMaster");
		else if (i == 1)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_EnergyExplotiation");
		else if (i == 2)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_FlameWave");
		else if (i == 3)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_IceSpear");
		else if (i == 4)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_IceStrike");
		else if (i == 5)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_LordOfCold");
		else if (i == 6)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_LordOfFlames");
		else if (i == 7)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_MagicArmor");
		else if (i == 8)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_SkillIcon_ManaDrift");
		else if (i == 9)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_SkillIcon_ThunderStorm");
		else if (i == 10)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_SkillIcon_Blind");

		StringCchPrintf(szTexture, _countof(szTextureName),
			L"Com_SkillIconTexture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureSkillIcon[i])))
			return E_FAIL;


		// 아이템 정보----------------------------------------------------------------
		INVEN_ITEM* pItem = new INVEN_ITEM;
		pItem->eSort = SKILL_ICON;
		if (i == 0)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"ElementalMaster");
		}
		if (i == 1)
		{
			pItem->eActiveID = ACTIVE_EXPLOTIATION;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"EnergyExplotiation");
		}
		if (i == 2)
		{
			pItem->eActiveID = ACTIVE_METEO;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"FlameWave");
		}
		if (i == 3)
		{
			pItem->eActiveID = ACTIVE_ICE_SPEAR;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"IceSpear");
		}
		if (i == 4)
		{
			pItem->eActiveID = ACTIVE_ICE_BLAST;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"IceStrike");
		}
		if (i == 5)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"LordOfCold");
		}
		if (i == 6)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"LordOfFlames");
		}
		if (i == 7)
		{
			pItem->eActiveID = ACTIVE_SHIELD;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"MagicArmor");
		}
		if (i == 8)
		{
			pItem->eActiveID = ACTIVE_MANA_DRIFT;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"ManaDrift");
		}
		if (i == 9)
		{
			pItem->eActiveID = ACTIVE_THUNDER_STORM;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"ThunderStorm");
		}
		if (i == 10)
		{
			pItem->eActiveID = ACTIVE_BLIND;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"Blind");
		}
		m_vSkillIconList.emplace_back(pItem);
	}
	return S_OK;
}

CDataManager * CDataManager::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CDataManager* pInstance = new CDataManager(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CItem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDataManager::Clone_GameObject(void * pArg)
{
	CDataManager* pInstance = new CDataManager(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CItem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDataManager::Free()
{
	for (_uint i = 0; i < 30; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pStatCom[i]);
	}

	for (auto& pItem : m_vItemList)
	{
		Safe_Delete(pItem);
	}
	m_vItemList.clear();

	for (_uint i = 0; i < 11; i++)
	{
		Safe_Release(m_pTextureSkillIcon[i]);
	}

	for (auto& pSkill : m_vSkillIconList)
		Safe_Delete(pSkill);
	m_vSkillIconList.clear();

	CGameObject::Free();
}
