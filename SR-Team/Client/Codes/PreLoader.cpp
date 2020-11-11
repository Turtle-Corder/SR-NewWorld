#include "stdafx.h"
#include "..\Headers\PreLoader.h"

#pragma region GameObject_Headers
#include "Terrain.h"
#include "Skybox.h"
#include "Slime.h"
#include "Snail.h"
#include "Stump.h"
#include "Yeti.h"
#include "Snow.h"
#include "Meteor.h"
#include "CubeTerrain.h"
#include "DropItem.h"
#include "Tree.h"
#include "Golem.h"
#include "Bomb.h"
#include "MonSub.h"
#include "Flower.h"
#include "PlaneSkill.h"
#include "EnergyBolt.h"
#include "Crack.h"
#include "TerrainBundle.h"
#include "Snail_Impact.h"
#include "Slime_Impact.h"
#include "Stump_Impact.h"
#include "Wand.h"
#include "Skybox.h"
#include "FlowerQuest_NPC.h"
#include "Shop_NPC.h"
#include "MainQuest_NPC.h"
#include "Stage1_NPC.h"
#include "Acorn.h"
#include "AcornExplosion.h"
#include "Wolf.h"
#include "Wolf_Impact.h"
#include "Golem_Impact.h"
#include "Fire.h"
#include "Stage2_NPC.h"
#include "Stage3_NPC.h"
#include "IceLandQuest.h"
#include "RandomBox_Npc.h"
#include "Rubble.h"
#include "Bomb_Residue.h"
#include "Blood.h"
#include "Dash_Piece.h"
#include "Golem_Dash.h"
#include "Golem_Piece.h"
#include "Stump_Particle.h"
#pragma endregion

USING(Client)



CPreLoader::CPreLoader(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
{
	Safe_AddRef(_pDevice);
}

_bool CPreLoader::IsFinished()
{
	return m_bFinished;
}

HRESULT CPreLoader::Setup_PreLoader(eSCENE_ID _eNextSceneID)
{
	m_eNextSceneID = _eNextSceneID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hLoadThread = (HANDLE)_beginthreadex(0, 0, ThreadMain, this, 0, 0);
	if (nullptr == m_hLoadThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Room()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

#pragma region GameObject_Skybox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ROOM, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ROOM, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Crack
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ROOM, L"GameObject_Crack", CCrack::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
#pragma region Component_Texture_Crack
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ROOM, L"Component_Texture_Crack", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/crack%d.png"))))
	{
	}
#pragma endregion

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 99)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Town()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//----------------------------------------------------------------------------------------------------
	// GameObject
	//----------------------------------------------------------------------------------------------------


#pragma region GameObject_Crack
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_Crack", CCrack::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Crack
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_Crack", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/crack%d.png"))))
	{
	}
#pragma endregion



#pragma region GameObject_Skybox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Snail
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_Snail", CSnail::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Snail_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_Snail_Impact", CSnail_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Dash_Piece
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_Dash_Piece", CDash_Piece::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_FlowerQuestNPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_FlowerQuestNPC", CFlowerQuest_NPC::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_ShopNPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_ShopNPC", CShop_NPC::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_MainQuestNPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_MainQuestNPC", CMainQuest_NPC::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_RandomBoxNPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_RandomBoxNPC", CRandomBox_Npc::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

//#pragma region GameObject_Acorn
//	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Acorn", CAcorn::Create(m_pDevice))))
//		return E_FAIL;
//#pragma endregion

#pragma region Component_Texture_Stump_Head
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_Stump_Head", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Head%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Stump_Part
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_Stump_Part", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Part%d.dds"))))
		return E_FAIL;
#pragma endregion

	//----------------------------------------------------------------------------------------------------
	// Component
	//----------------------------------------------------------------------------------------------------

	// terrain

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_TOWN, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


	// FlowerQuest_NPC
#pragma region Component_Texture_FlowerQuestNPC
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_FlowerQuestNPCHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/FlowerQuest_Head%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_FlowerQuestNPCBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/FlowerQuest_Body%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_FlowerQuestNPCLeftHand", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/FlowerQuest_LeftHand%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_FlowerQuestNPCRightHand", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/FlowerQuest_RightHand%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_FlowerQuestNPCFoot", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/FlowerQuest_Foot%d.dds"))))
		return E_FAIL;
#pragma endregion

	// Shop_NPC
#pragma region Component_Texture_ShopNPC
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_ShopNPCHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/Shopper_Head%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_ShopNPCBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/Shopper_Body%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_ShopNPCHand", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/Shopper_Hand%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_ShopNPCFoot", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/Shopper_Foot%d.dds"))))
		return E_FAIL;
#pragma endregion

	// MainQuest_NPC
#pragma region Component_Texture_MainQuestNPC
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_MainQuestNPCHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/MainQuestNPC_Head%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_MainQuestNPCBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/MainQuestNPC_Body%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_MainQuestNPCHand", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/MainQuestNPC_Hand%d.dds"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TOWN, L"Component_Texture_MainQuestNPCFoot", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_NPC/MainQuestNPC_Foot%d.dds"))))
		return E_FAIL;
#pragma endregion

// 0번 마을

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 0)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Forest()
{
	//숲
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
#pragma region Snail

	
#pragma region GameObject_Crack
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Crack", CCrack::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Crack
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Crack", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/crack%d.png"))))
	{
	}
#pragma endregion


#pragma region GameObject_Skybox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Snail
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Snail", CSnail::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Snail_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Snail_Impact", CSnail_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Dash_Piece
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Dash_Piece", CDash_Piece::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SnailBody
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_SnailBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SnailBody%d.dds"))))
	{
	}
#pragma endregion

#pragma region Component_Texture_SnailHead
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_SnailHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SnailHead%d.dds"))))
	{
	}

#pragma region Component_Texture_Dash_Piece
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Dash_Piece", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Dash_Piece%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma endregion



#pragma endregion

#pragma region Slime

#pragma region GameObject_Slime
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Slime", CSlime::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Slime_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Slime_Impact", CSlime_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Translucent_Cube
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Slime", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Test/TranslucentCube%d.dds", 6))))
	{
	}
#pragma endregion

#pragma region Component_Texture_Crack
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Crack", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/crack%d.png"))))
	{
	}
#pragma endregion


#pragma endregion

#pragma region Stump

#pragma region GameObject_Stump
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Stump", CStump::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Acorn
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Acorn", CAcorn::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Rubble
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Rubble", CRubble::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Stump_Particle
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Stump_Particle", CStump_Particle::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Stump_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Stump_Impact", CStump_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Acorn_Explosion
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_AcornExplosion", CAcornExplosion::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Stump_Head
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Stump_Head", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Head%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Stump_Part
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Stump_Part", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Part%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Acorn
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Acorn", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stone%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_AcornExplosion
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_AcornExplosion", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stone%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Stump_Particle
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Stump_Particle", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Bomb_Residue/Bomb_Residue%d.dds", 4))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Stage1NPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_FOREST, L"GameObject_Stage1NPC", CStage1_NPC::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Stage1NPC_WND
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_FOREST, L"Component_Texture_Stage1NPC_WND", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/Sprite/Layer_MainQuest/stage1_npc%d.png"))))
		return E_FAIL;
#pragma endregion

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Iceland()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

#pragma region Yeti


#pragma region GameObject_Crack
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ICELAND, L"GameObject_Crack", CCrack::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Crack
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Crack", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/crack%d.png"))))
	{
	}
#pragma endregion


#pragma region GameObject_Yeti
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ICELAND, L"GameObject_Yeti", CYeti::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiBody
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_YetiBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiBody%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiHead
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_YetiHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiHead%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiPart
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_YetiPart", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiPart%d.dds"))))
		return E_FAIL;
#pragma endregion

	// Snow
#pragma region GameObject_Snow
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ICELAND, L"GameObject_Snow", CSnow::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Snow", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Snow%d.dds"))))
		return E_FAIL;

#pragma endregion


#pragma region Wolf

#pragma region GameObject_Wolf
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ICELAND, L"GameObject_Wolf", CWolf::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Wolf_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ICELAND, L"GameObject_Wolf_Impact", CWolf_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Blood
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ICELAND, L"GameObject_Blood", CBlood::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Wolf_Face
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Wolf_Face", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Wolf_Face%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Wolf_Mouth
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Wolf_Mouth", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Wolf_Mouth%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Wolf_Ear
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Wolf_Ear", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Wolf_Ear%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Wolf_Neck
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Wolf_Neck", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Wolf_Neck%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Wolf_Body
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Wolf_Body", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Wolf_Body%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Wolf_Leg
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Wolf_Leg", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Wolf_Leg%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Blood
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_ICELAND, L"Component_Texture_Blood", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Blood%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ICELAND, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Stage2NPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_ICELAND, L"GameObject_Stage2NPC", CStage2_NPC::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 2)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Volcanic()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;



#pragma region GameObject_Crack
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_Crack", CCrack::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Crack
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_Crack", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/crack%d.png"))))
	{
	}
#pragma endregion

#pragma region GameObject_Golem
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_Golem", CGolem::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Golem_Dash
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_Golem_Dash", CGolem_Dash::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_GolemBody
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_BossBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/BossBody%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_GolemHead
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_BossHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/BossHead%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_GolemPart
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_BossPart", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/BossPart%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Bomb
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_Bomb", CBomb::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Bomb_Residue
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_Bomb_Residue", CBomb_Residue::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Bomb_Residue
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_Bomb_Residue", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Bomb_Residue/Bomb_Residue%d.dds", 4))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Bomb
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_Bomb", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Bomb/Bomb%d.dds", 3))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_MonSub
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_MonSub", CMonSub::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_MonSub
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_MonSub", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/MonSub%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Fire
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_Fire", CFire::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Fire
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_Fire", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Fire/Fire%d.png", 11))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Golem_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_Golem_Impact", CGolem_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_GolemImpact
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_Wolf_Face", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Wolf_Face%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Golem_Dash
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_VOLCANIC, L"Component_Texture_Golem_Dash", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Dash%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Golem_Piece
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_Golem_Piece", CGolem_Piece::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_VOLCANIC, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Stage3NPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE3, L"GameObject_Stage3NPC", CStage3_NPC::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 3)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Lab()
{
	return S_OK;
}

_uint CPreLoader::ThreadMain(void * _pParam)
{
	CPreLoader* pInstance = (CPreLoader*)_pParam;
	if (nullptr == pInstance)
		return 1;

	HRESULT hr = 0;
	
	EnterCriticalSection(&pInstance->m_CriticalSection);
	switch (pInstance->m_eNextSceneID)
	{
	case eSCENE_ID::SCENE_STATIC:
		break;

	case eSCENE_ID::SCENE_LOGO:
		break;

	case eSCENE_ID::SCENE_ROOM:
		hr = pInstance->Load_Resources_Room();
		break;

	case eSCENE_ID::SCENE_STAGE0:
		hr = pInstance->Load_Resources_Town();
		break;

	case eSCENE_ID::SCENE_STAGE1:
		hr = pInstance->Load_Resources_Forest();
		break;

	case eSCENE_ID::SCENE_STAGE2:
		hr = pInstance->Load_Resources_Iceland();
		break;

	case eSCENE_ID::SCENE_STAGE3:
		hr = pInstance->Load_Resources_Volcanic();
		break;

	case eSCENE_ID::SCENE_STAGE4:
		hr = pInstance->Load_Resources_Lab();
		break;

	default:
		break;
	}

	LeaveCriticalSection(&pInstance->m_CriticalSection);
	if (FAILED(hr))
	{
		PRINT_LOG(L"Failed To Resource Load", LOG::DEBUG);
		return 1;
	}

	pInstance->m_bFinished = true;
	return 0;
}

CPreLoader * CPreLoader::Create(LPDIRECT3DDEVICE9 _pDevice, eSCENE_ID _eNextSceneID)
{
	if (nullptr == _pDevice)
		return nullptr;

	CPreLoader* pInstance = new CPreLoader(_pDevice);
	if (FAILED(pInstance->Setup_PreLoader(_eNextSceneID)))
	{
		PRINT_LOG(L"Failed To Create CPreLoader", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPreLoader::Free()
{
	if (!m_bFinished)
		WaitForSingleObject(m_hLoadThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hLoadThread);

	Safe_Release(m_pDevice);
}





HRESULT CPreLoader::Setup_Stage_CubeTerrain(const wstring & LayerTag, const _uint& StageNumber)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	wifstream fin;
	TCHAR szFilePath[MID_STR] = _T("");

	_int iToScene = 0;

	TCHAR szFilePathReady[MID_STR] = _T("../DataPath/MapSource/Stage%d.txt");
	StringCchPrintf(szFilePath, _countof(szFilePath), szFilePathReady, StageNumber); //상수로 때려박아야 함

	if (99 == StageNumber)
		iToScene = (_int)SCENE_ROOM;
	else
		iToScene = (_int)(StageNumber + 3);

	
	fin.open(szFilePath);
	if (!fin.fail())
	{
		//변수
		TCHAR szXVerCount[MAX_PATH] = L"";
		TCHAR szZVerCount[MAX_PATH] = L"";
		TCHAR szFloor[MAX_PATH] = L"";
		TCHAR szIndex[MAX_PATH] = L"";
		TCHAR szOnOff[MAX_PATH] = L"";
		TCHAR szTextureID[MAX_PATH] = L"";
		TCHAR szMovePossible[10] = L"";

		//X, Z 축갯수
		fin.getline(szXVerCount, MAX_PATH, L'|');
		fin.getline(szZVerCount, MAX_PATH);

		_int XNumber = _ttoi(szXVerCount) - 1;
		_int ZNumber = _ttoi(szZVerCount) - 1;

		TILEINFO* tTileInfo = new TILEINFO[XNumber * ZNumber];

		int iFloorMax = XNumber*ZNumber;

		if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_TerrainBundle", iToScene, _T("Layer_TerrainBundle"), &iFloorMax)))
			return E_FAIL;

		while (true)
		{
			//Cube Info 받기
			fin.getline(szFloor, MAX_PATH, L'|');
			fin.getline(szIndex, MAX_PATH, L'|');
			fin.getline(szOnOff, MAX_PATH, L'|');
			fin.getline(szTextureID, MAX_PATH, L'|');
			fin.getline(szMovePossible, MAX_PATH);
			_uint iFloor = _ttoi(szFloor);
			_uint iIndex = _ttoi(szIndex);
			_bool bOnOff = 1 && (_ttoi(szOnOff));
			_uint iTextureID = _ttoi(szTextureID);

			//Index 구하기
			_uint iXIndex = iIndex % XNumber;
			_uint iZIndex = iIndex / XNumber;

			//넘겨줄 TerrainInfo
			TERRAININFO Temp_Info;
			Temp_Info.iFloor = iFloor;
			Temp_Info.iIndex = iIndex;
			Temp_Info.iTextureID = iTextureID;
			Temp_Info.iX_Index = iXIndex;
			Temp_Info.iZ_Index = iZIndex;
			Temp_Info.iInterval = 2.f;
			Temp_Info.iMaxX = XNumber;
			Temp_Info.iMaxZ = ZNumber;


			tTileInfo[iIndex].iOpt = 0;			// 길 찾으려면 onoff를 넣어줘야?
			tTileInfo[iIndex].iX = iXIndex;
			tTileInfo[iIndex].iZ = iZIndex;



			if (fin.eof())
				break;

			CGameObject* pOut = nullptr;

			if (true == bOnOff)
			{
				if (FAILED(pManagement->Add_GameObject_InLayer(&pOut, iToScene, L"GameObject_CubeTerrain", iToScene, LayerTag, &Temp_Info)))
					return E_FAIL;
			}

			Temp_Info.pObj = pOut;

			((CTerrainBundle*)pManagement->Get_GameObject(iToScene, _T("Layer_TerrainBundle")))->Set_TerrainInfo(iIndex, iFloor, Temp_Info);

		}

		pManagement->Set_TileInfo(tTileInfo, XNumber, ZNumber);
	}

	else
		return E_FAIL;


	fin.close();
	return S_OK;
}
