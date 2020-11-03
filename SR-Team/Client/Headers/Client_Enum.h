#pragma once
#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

enum eSCENE_ID
{
	SCENE_STATIC,
	SCENE_LOGO,
	SCENE_STAGE0,
	SCENE_STAGE1,
	SCENE_STAGE2,
	SCENE_STAGE3,
	SCENE_STAGE4,
	SCENE_STAGE5,
	SCENE_END,
};





//----------------------------------------------------------------------------------------------------
// 인간 형태 파츠
//----------------------------------------------------------------------------------------------------
enum eHUMANOID_PART
{
	PART_START,
	PART_HEAD = PART_START,
	
	PART_BODY,
	
	PART_HAND_START,
	PART_HAND_LEFT = PART_HAND_START, 
	PART_HAND_RIGHT,
	PART_HAND_END = PART_HAND_RIGHT,
	
	PART_FOOT_START,
	PART_FOOT_LEFT = PART_FOOT_START, 
	PART_FOOT_RIGHT,
	PART_FOOT_END = PART_FOOT_RIGHT,
	
	PART_END
};





//----------------------------------------------------------------------------------------------------
// 플레이어 스킬 ID
//----------------------------------------------------------------------------------------------------
enum ePLAYER_SKILL_ID
{
	PLAYER_NORMAL_ATTACK,
	PLAYER_SKILL_LASER,
	PLAYER_SKILL_FALL,
	PLAYER_SKILL_END
};





//----------------------------------------------------------------------------------------------------
// 메인 UI
//----------------------------------------------------------------------------------------------------
enum eMAINUI_ID
{
	MAINUI_START, 
	MAINUI_MAIN = MAINUI_START,
	MAINUI_HP, MAINUI_MP,
	MAINUI_QUICKSLOT_LFFT, MAINUI_QUICKSLOT_RIGHT,
	MAINUI_END
};


//--------------------------------------------------
// 상점 UI
//--------------------------------------------------
enum eSHOP_ID
{
	SHOP_START,
	SHOP_WND = SHOP_START,
	SHOP_SCROLLBAR,
	SHOP_END
};


//--------------------------------------------------
// 인벤토리 UI
//--------------------------------------------------
enum eINVEN_ID
{
	INVEN_START,
	INVEN_WND = INVEN_START,
	INVEN_SELL_BUTTON,
	INVEN_GOLD,
	INVEN_END
};

//--------------------------------------------------
// 장비창 UI
//--------------------------------------------------
enum eEQUIP_ID
{
	EQUIP_START,
	EQUIP_WND = EQUIP_START, 
	EQUIP_EQUIPMENT,
	EQUIP_INFO,
	EQUIP_END
};


//--------------------------------------------------
// 스킬창 UI
//--------------------------------------------------
enum eSKILL_ID
{
	SKILL_START,
	SKILL_ACTIVE_WND = SKILL_START,
	SKILL_PASSIVE_WND,
	SKILL_END
};


//--------------------------------------------------
// 액티브 스킬 종류
//--------------------------------------------------
enum eActiveSkill_ID
{
	ACTIVE_ICE_STRIKE, 
	ACTIVE_MANA_DRIFT, 
	ACTIVE_ENERGY_EXPLOTIATION, 
	ACTIVE_FLAME_WAVE,
	ACTIVE_ICE_SPEAR,
	ACTIVE_MAGIC_ARMOR, 
	ACTIVE_NORMAL_ATTACK,
	ACTIVE_SKILL,
	ACTIVE_SKILL_END
};


//--------------------------------------------------
// 포션 아이템 종류
//--------------------------------------------------
enum ePotion_ID
{
	RED_POTION, RED_ELIXIR,
	BLUE_POTION, BLUE_ELIXIR,
	POTION_END
};


//--------------------------------------------------
// 아이템 종류
//--------------------------------------------------
enum eITEM_SORT
{
	CAP, NECKLACE1, TOP, PANTS, SUIT, STAFF1, 
	EARRING, CLOAK, GLOVES, NECKLACE2, SHOES, STAFF2,
	ITEMSORT_END, 
	POTION, SKILL_ICON, QUEST1
};


//--------------------------------------------------
// 공격 속성
//--------------------------------------------------
enum eELEMENTAL_TYPE
{
	NONE,
	FIRE,
	ICE,
	ELEMENTAL_END
};


//--------------------------------------------------
// 첫 번째 퀘스트 상황
//--------------------------------------------------
enum eQuest1_ID
{
	GREETINGS, REQUEST,
	REJECT, ACCEPT,
	CLEAR, NOCLEAR,
	FINAL,
	QUEST1_END,
	QUEST1_WAIT,
	QUEST1_CLEARWAIT,
	QUEST1_FINISH
};

#endif
