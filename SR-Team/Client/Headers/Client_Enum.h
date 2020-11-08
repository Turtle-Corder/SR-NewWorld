#pragma once
#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

enum eSCENE_ID
{
	SCENE_STATIC,
	SCENE_LOGO,
	
	SCENE_ROOM,

	SCENE_TOWN,
	SCENE_STAGE0 = SCENE_TOWN,

	SCENE_FOREST,
	SCENE_STAGE1 = SCENE_FOREST,

	SCENE_ICELAND,
	SCENE_STAGE2 = SCENE_ICELAND,
	
	SCENE_VOLCANIC,
	SCENE_STAGE3 = SCENE_VOLCANIC,
	
	SCENE_LAB,
	SCENE_STAGE4 = SCENE_LAB,
	
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
	ACTIVE_METEO,
	ACTIVE_ICE_SPEAR,
	ACTIVE_THUNDER_STORM,
	ACTIVE_MANA_DRIFT,
	ACTIVE_BLIND,
	ACTIVE_SHIELD,
	ACTIVE_ICE_BLAST,
	ACTIVE_EXPLOTIATION,

	//ACTIVE_ICE_BLAST,
	//ACTIVE_MANA_DRIFT,
	//ACTIVE_ENERGY_EXPLOTIATION,
	//ACTIVE_FLAME_WAVE,
	//ACTIVE_ICE_SPEAR,
	//ACTIVE_MAGIC_ARMOR,
	//ACTIVE_THUNDER_STORM,
	//ACTIVE_BLIND,
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
	POTION, SKILL_ICON, QUEST1, MAIN_QUEST
};


//--------------------------------------------------
// 공격 속성
//--------------------------------------------------
enum eELEMENTAL_TYPE
{
	NONE,
	FIRE,
	ICE,
	EARTH,
	LIGHTNING,
	DARK,
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


//--------------------------------------------------
// 꽃 퀘스트 상황
//--------------------------------------------------
enum eFlowerQuest_ID
{
	/*FLOWER_GREETINGS,
	FLOWER_QUESTION, FLOWER_ANSWER, FLOWER_QUEST_START,
	FLOWER_QUEST_CLEAR, FLOWER_REWARD,
	FLOWER_EXTRA_QUESTION,
	FLOWER_QUEST_NOCLEAR,
	FLOWER_QUEST_END,*/

	FLOWER_GREETINGS,
	FLOWER_ANSWER1,
	FLOWER_CHAT,
	FLOWER_ANSWER2,
	FLOWER_REQUIRE_QUEST,
	FLOWER_REJECT,
	FLOWER_QUEST_NOCLEAR,
	//FLOWER_QUEST_CLEAR,
	FLOWER_REWARD,
	FLOWER_QUEST_END,

	FLOWER_ON_THE_QUEST,
	FLOWER_FINISH,
	FLOWER_REAL_FINISH
};


//--------------------------------------------------
// 메인 퀘스트 상황
//--------------------------------------------------
enum eMainQuest_ID
{
	MAINQUEST_GREETINGS,
	MAINQUEST_ASK1,
	MAINQUEST_ANSWER1,
	MAINQUEST_ASK2,
	MAINQUEST_ANSWER2,
	MAINQUEST_ASK_QUEST,
	MAINQUEST_AGREE,
	MAINQUEST_REJECT,
	MAINQUEST_NO_CLEAR,
	MAINQUEST_REWARD,
	MAINQUEST_END,

	MAINQUEST_ON_THE_QUEST,
	MAINQUEST_FINISH,
	MAINQUEST_REAL_FINISH
};


//--------------------------------------------------
// 상점 ChatWnd 상황
//--------------------------------------------------
enum eShopChatWnd_ID
{
	SHOPCHAT_GREETINGS,
	SHOPCHAT_REQUIRE,
	SHOPCHAT_ANSWER,
	SHOPCHAT_FINAL,
	SHOPCHAT_END,

	SHOPCHAT_FINISH
};


//--------------------------------------------------
// NPC ID
//--------------------------------------------------
enum eNormalNPC_ID
{
	STAGE1_NPC,
	STAGE3_NPC,

	STAGE_NPC_END
};

#endif
