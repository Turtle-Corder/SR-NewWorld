#include "stdafx.h"
#include "Scene_Logo.h"
#include "SkillSlotManager.h"
#include "ItemSlotManager.h"
#include "..\Headers\MainApp.h"


#pragma region Scene_Headers
#include "Scene_Logo.h"
#pragma endregion


#pragma region GameObject_Headers
#include "Player.h"
#include "Wand.h"
#include "MainCamera.h"
#include "UICamera.h"
#include "MainUI.h"
#include "Shop.h"
#include "DataManager.h"
#include "DropItem.h"
#include "Inventory.h"
#include "Equip.h"
#include "Skill.h"
#include "TerrainBundle.h"
#include "SkillInven.h"
#include "SkillSlot_Meteor.h"
#include "EnergyBolt.h"
#include "Meteor.h"

#include "SkillSlot_IceStrike.h"
#include "ItemInventory.h"
#include "ItemSlot_RedPotion.h"
#include "Mouse.h"
#include "ItemSlot_BluePotion.h"
#include "ItemSlot_RedElixir.h"
#include "ItemSlot_BlueElixir.h"
#include "SkillSlot_ManaDrift.h"
#include "SkillSlot_EnergyExlposion.h"
#include "Tree.h"
#include "Flower.h"
//#include "Quest1.h"
#pragma endregion

#pragma region Component_Headers
#include "Status.h"
#include "DamageInfo.h"

#include "VIBuffer_TreeHead.h"
#include "VIBuffer_Flower.h"
#include "VIBuffer_Pyramid.h"
#pragma endregion


USING(Client)

CMainApp::CMainApp()
	: m_pManagement(CManagement::Get_Instance())
{
	Safe_AddRef(m_pManagement);
}

HRESULT CMainApp::Setup_MainApp()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->Setup_Engine(g_hWnd, FRAME_PER_SEC, WINCX, WINCY, CDevice_Manager::DISPLAY_WINDOW, SCENE_END, MAINAPP_TIMER)))
	{
		PRINT_LOG(L"Failed To Setup Engine", LOG::CLIENT);
		return E_FAIL;
	}

	// device
	m_pDevice = m_pManagement->Get_Device();
	if (nullptr == m_pDevice)
	{
		PRINT_LOG(L"Device is nullptr", LOG::CLIENT);
		return E_FAIL;
	}

	Safe_AddRef(m_pDevice);

	// sprite
	m_pSprite = m_pManagement->Get_Sprite();
	if (nullptr == m_pSprite)
	{
		PRINT_LOG(L"Sprite is nullptr", LOG::CLIENT);
		return E_FAIL;
	}

	Safe_AddRef(m_pSprite);

	// font
	m_pFont = m_pManagement->Get_Font();
	if (nullptr == m_pFont)
	{
		PRINT_LOG(L"Font is nullptr", LOG::CLIENT);
		return E_FAIL;
	}

	Safe_AddRef(m_pFont);

	if (FAILED(Setup_DefaultSetting()))
	{
		PRINT_LOG(L"Failed To Setup_DefaultSetting", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(Setup_StaticResources()))
	{
		PRINT_LOG(L"Failed To Setup_StaticResources", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(Setup_ProtoTypeData()))
	{
		PRINT_LOG(L"Failed To Setup_SaveData", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(m_pManagement->Change_CurrentScene(SCENE_LOGO, CScene_Logo::Create(m_pDevice))))
	{
		PRINT_LOG(L"Failed To Change_CurrentScene SCENE_LOGO", LOG::CLIENT);
		return E_FAIL;
	}


	//ShowCursor(FALSE);

	return S_OK;
}

_int CMainApp::Update_MainApp()
{
	if (nullptr == m_pManagement)
		return 0;

	_int iBehaviour = m_pManagement->Update_Engine();

	//_crtBreakAlloc = 18;
	return iBehaviour;
}

HRESULT CMainApp::Render_MainApp()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->Render_Engine()))
	{
		PRINT_LOG(L"Failed To Render_Engine", LOG::CLIENT);
		return E_FAIL;
	}

	_crtBreakAlloc = 18;
	return S_OK;
}

_bool CMainApp::Lock_FrameManager()
{
	if (nullptr == m_pManagement)
		return false;

	return m_pManagement->Lock_FrameManager();
}

void CMainApp::Render_FrameManager()
{
	if (nullptr == m_pManagement)
		return;

	return m_pManagement->Render_FrameManager();

}

HRESULT CMainApp::Setup_DefaultSetting()
{
	srand(unsigned(time(NULL)));

	if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		PRINT_LOG(L"Failed To Light Off", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Setup_StaticResources()
{
	//----------------------------------------------------------------------------------------------------
	// Sound
	//----------------------------------------------------------------------------------------------------
#pragma region Sound_
#pragma endregion

	if (FAILED(Setup_HK()))
	{
		PRINT_LOG(L"Failed To Setup HK", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(Setup_EB()))
	{
		PRINT_LOG(L"Failed To Setup EB", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(Setup_YJ()))
	{
		PRINT_LOG(L"Failed To Setup YJ", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(Setup_DS()))
	{
		PRINT_LOG(L"Failed To Setup DS", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Setup_HK()
{
#pragma region GameObject_Mouse
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Mouse", CMouse::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Textures_Mouse

	// idle
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Mouse_Idle", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/2DResource/Mouse/idle/idle%d.png"))))
		return E_FAIL;

	// grab
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Mouse_Grab", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/2DResource/Mouse/grab/grab%d.png"))))
		return E_FAIL;

	// help
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Mouse_Help", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/2DResource/Mouse/help/help%d.png"))))
		return E_FAIL;

	// click
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Mouse_Click", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/2DResource/Mouse/click/click%d.png", 2))))
		return E_FAIL;

	// work
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Mouse_Work", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/2DResource/Mouse/work/work%d.png", 4))))
		return E_FAIL;

#pragma endregion

	//----------------------------------------------------------------------------------------------------
	// GameObject
	//----------------------------------------------------------------------------------------------------

#pragma region GameObject_MainCamera
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_MainCamera", CMainCamera::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_UICamera
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_UICamera", CUICamera::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


	//----------------------------------------------------------------------------------------------------
	// Component
	//----------------------------------------------------------------------------------------------------
#pragma region Component_VIBuffer_RectTextrue
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_RectTexture", CVIBuffer_RectTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_CubeTextrue
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", CVIBuffer_CubeTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Transform
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Raycast
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Raycast", CRaycast::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Collider_Sphere
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_Sphere", CSphereCollider::Create(m_pDevice))))
		return E_FAIL;
#pragma  endregion

#pragma region Component_Collider_Box
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_Box", CBoxCollider::Create(m_pDevice))))
		return E_FAIL;
#pragma  endregion

#pragma region Component_Status
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Status", CStatus::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_DamageInfo
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_DamageInfo", CDamageInfo::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Setup_YJ()
{
#pragma region Staic

#pragma region GameObject_EnergyBolt
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_EnergyBolt", CEnergyBolt::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Meteor
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Meteor", CMeteor::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Meteor
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Meteor", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Meteor%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_EnergyBolt
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_EnergyBolt", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/EnergyBolt%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region Component_Texture_DropDiamond
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_DropDiamond", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/item/diamond%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_DropRuby
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_DropRuby", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/item/ruby%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_ironsword
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Dropiron_sword", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/item/iron_sword%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_DropItem
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_DropItem", CDropItem::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Wand
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Wand", CWand::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Setup_EB()
{
#pragma region GameObject_Player
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Player", CPlayer::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_MainUI
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_MainUI", CMainUI::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Shop
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Shop", CShop::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Item 
	// DataManager
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Item", CDataManager::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Inven
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Inven", CInventory::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Equip
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Equip", CEquip::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Skill
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Skill", CSkill::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_SkillInven
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_SkillInven", CSkillInven::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_ItemInven
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_ItemInven", CItemInventory::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

// UNDONE : Quest Prototype
//#pragma region GameObject_Quest1
//	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Quest1", CQuest1::Create(m_pDevice, m_pSprite, m_pFont))))
//		return E_FAIL;
//#pragma endregion

#pragma region Component_Textures_MainUI

	// frame
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_Main", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/MainUI임%d.png"))))
		return E_FAIL;

	// hp
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_Hp", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/HPbar%d.png"))))
		return E_FAIL;

	// mp
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_Mp", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/MpBar%d.png"))))
		return E_FAIL;

	// slot_left
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_QuickSlot_Left", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/QuickSlot_Right%d.png"))))
		return E_FAIL;

	// slot_left
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_QuickSlot_Right", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/QuickSlot_Right%d.png"))))
		return E_FAIL;

#pragma endregion

#pragma region Component_Textures_Shop
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Shop_ShopWnd", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Shop/ShopWnd%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Shop_ShopScrollBar", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Shop/ScrollBar%d.png"))))
		return E_FAIL;
#pragma endregion


#pragma region Component_Textures_Inventory
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Inven_InvenWnd", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Inventory/Inven%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Inven_SellButton", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Inventory/SellButton%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_Empty", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Inventory/empty%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Select_SellItem", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/select_sellitem%d.png"))))
		return E_FAIL;
#pragma endregion


#pragma region Component_Textures_Equip
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Equip_Stat", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/Stat%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Equip_Equipment", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/Equip%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Equip_BackGround", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/back%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Equip_Info", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainUI/info%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Skill
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Skill_ActiveWnd", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/2DResource/Skill/active%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Skill_PassiveWnd", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/2DResource/Skill/passive%d.png"))))
		return E_FAIL;
#pragma endregion


#pragma region Component_Textures_SkillIcon
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_ElementalMaster", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/elemental_master%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_EnergyExplotiation", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/energy_exploitation%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_FlameWave", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/flame_wave%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_IceSpear", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/ice_spear%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_IceStrike", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/ice_strike%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_LordOfCold", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/lord_of_cold%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_LordOfFlames", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/lord_of_flames%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_MagicArmor", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/magic_armor%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillIcon_ManaDrift", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Skill_Icon/mana_drift%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SkillInfo
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillInfo_EnergyExploitiation", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_MainUI/energy_exploitiation%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillInfo_FlameWave", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_MainUI/flame_wave%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillInfo_IceSpear", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_MainUI/ice_spear%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillInfo_IceStrike", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_MainUI/ice_strike%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillInfo_MagicArmor", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_MainUI/magic_armor%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_SkillInfo_ManaDrift", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_MainUI/mana_drift%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Textures_Item

#pragma region Shoes
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_ScholarShoes",
		CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE, L"../Resources/Sprite/Layer_Item/Scholar_Shoes%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_ArcaneShoes",
		CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE, L"../Resources/Sprite/Layer_Item/Moonlight_Traveler's_Shoes%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Staff
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_GoldenSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/golden_sword%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_IronSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/iron_sword%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_DiaSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/diamond_sword%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Belt, Gloves, Wing
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_Item_AbsoluteBelt", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_Item/absolute_belt%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_Item_AquaGloves", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_Item/aqua_gloves%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_Item_BalrogWings", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_Item/balrog_wings%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_Item_SorcererGloves", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_Item/high_sorcerer_gloves%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Potion
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_RedPotion", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/red_potion%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_WhitePotion", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/white_potion%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_OrangePotion", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/orange_potion%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_Item_BluePotion", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_Item/blue_potion%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_Item_RedElixir", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_Item/red_elixir%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC,
		L"Component_Texture_Item_BlueElixir", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/Sprite/Layer_Item/blue_elixir%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Suit
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_BlackDress", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/black_dress%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_PupleDress", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Item/puple_dress%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Quest1
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Quest1_One", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Quest1/one%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Quest1_Two", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Quest1/two%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Quest1_ThreeNo", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Quest1/three_no%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Quest1_ThreeOk", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Quest1/three_ok%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Quest1_FourClear", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Quest1/four_clear%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Quest1_FourNoClear", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Quest1/four_noclear%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Quest1_Goguma", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Quest1/goguma%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Quest1_Final", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_Quest1/final%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Player
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Player", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_Player/player_head%d.dds"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_PlayerHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_Player/player_body%d.dds"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_PlayerHand", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_Player/player_hand%d.dds"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_PlayerFoot", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_Player/player_foot%d.dds"))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Setup_DS()
{
#pragma region Component_VIBuffer_TreeHead
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_TreeHead", CVIBuffer_TreeHead::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Tree
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Tree", CTree::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_Flower
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_Flower", CVIBuffer_Flower::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Flower
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Flower", CFlower::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_Pyramid
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_Pyramid", CVIBuffer_Pyramid::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_TerrainBundle
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_TerrainBundle", CTerrainBundle::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Setup_ProtoTypeData()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	wifstream fin;
	fin.open("../DataPath/3DPath.txt");
	if (!fin.fail())
	{
		TCHAR szCount[3] = L"";
		while (true)
		{
			//---------------------------------------------------------------
			//FILEINFO 로딩
			//---------------------------------------------------------------


			MYFILEINFO pFileInfo;

			fin.getline(pFileInfo.wstrFilePath, MAX_PATH, L'|');
			fin.getline(pFileInfo.wstrDimenKey, MAX_PATH, L'|');
			fin.getline(pFileInfo.wstrLayerKey, MAX_PATH, L'|');
			fin.getline(pFileInfo.wstrObjectKey, MAX_PATH, L'|');
			fin.getline(pFileInfo.wstrSaveTypeKey, MAX_PATH, L'|');

			fin.getline(szCount, MAX_PATH);
			pFileInfo.iCount = _ttoi(szCount);

			//끝나면 탈출
			if (fin.eof())
				break;



			//---------------------------------------------------------------
			//처리용 문자열 세팅
			//---------------------------------------------------------------

		
			wstring wstrCombine = L"";
			wstring wstrObjKey = pFileInfo.wstrObjectKey;



			TCHAR *ptr = wcschr(pFileInfo.wstrObjectKey, L'_');
			wprintf(L"%s\n", ptr);

			wstrCombine = L"Component_Texture" + wstring(ptr);


			ptr = nullptr;

			delete ptr;


			wstring wstrFullPath = pFileInfo.wstrFilePath;

			wsprintf(szCount, L"%d", pFileInfo.iCount - 1);




			//---------------------------------------------------------------
			//확장자 합성
			////---------------------------------------------------------------
			//if (!wcscmp(pFileInfo.wstrSaveTypeKey, L"DDS"))
			//	wstrFullPath = wstrFullPath.substr(0, wstrFullPath.length() - 6) + szNowCount + L".dds";

			//else if (!wcscmp(pFileInfo.wstrSaveTypeKey, L"PNG"))
			//	wstrFullPath = wstrFullPath.substr(0, wstrFullPath.length() - 6) + szNowCount + L".png";




			//---------------------------------------------------------------
			//ProtoType 추가
			//---------------------------------------------------------------

			if (!wcscmp(pFileInfo.wstrSaveTypeKey, L"DDS"))
			{
				if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, wstrCombine.c_str(), CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
					wstrFullPath, pFileInfo.iCount))))
					return E_FAIL;
			}
			else if (!wcscmp(pFileInfo.wstrSaveTypeKey, L"PNG") && !wcscmp(pFileInfo.wstrDimenKey, L"3D"))
			{
				if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, wstrCombine.c_str(), CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL,
					wstrFullPath, pFileInfo.iCount))))
					return E_FAIL;
			}
			else if (!wcscmp(pFileInfo.wstrSaveTypeKey, L"PNG") && !wcscmp(pFileInfo.wstrDimenKey, L"Sprite"))
			{
				if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, wstrCombine.c_str(), CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
					wstrFullPath, pFileInfo.iCount))))
					return E_FAIL;
			}
			
		}
	}
	fin.close();

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->Setup_MainApp()))
	{
		PRINT_LOG(L"Failed To Create CMainApp", LOG::CLIENT);
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CMainApp::Free()
{
	CSkillSlotManager::Destroy_Instance();
	CItemSlotManager::Destroy_Instance();

	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);

	//ShowCursor(TRUE);

	if (CManagement::Release_Engine())
		PRINT_LOG(L"Failed To Release Engine", LOG::CLIENT);
}