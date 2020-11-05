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
#include "Wand.h"
#include "Skybox.h"
#include "FlowerQuest_NPC.h"
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

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 99)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage0()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//----------------------------------------------------------------------------------------------------
	// GameObject
	//----------------------------------------------------------------------------------------------------

#pragma region GameObject_Skybox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE0, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Snail
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE0, L"GameObject_Snail", CSnail::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Snail_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE0, L"GameObject_Snail_Impact", CSnail_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Stump
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE0, L"GameObject_Stump", CStump::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_FlowerQuestNPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE0, L"GameObject_FlowerQuestNPC", CFlowerQuest_NPC::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

//#pragma region GameObject_Acorn
//	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Acorn", CAcorn::Create(m_pDevice))))
//		return E_FAIL;
//#pragma endregion

#pragma region Component_Texture_Stump_Head
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE0, L"Component_Texture_Stump_Head", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Head%d.dds"))))
	{
	}
#pragma endregion

#pragma region Component_Texture_Stump_Part
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE0, L"Component_Texture_Stump_Part", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Part%d.dds"))))
	{
	}
#pragma endregion

	//----------------------------------------------------------------------------------------------------
	// Component
	//----------------------------------------------------------------------------------------------------

	// terrain

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE0, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


	// FlowerQuest_NPC
#pragma region Component_Texture_FlowerQuestNPC
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE0, L"Component_Texture_FlowerQuestNPCHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_FlowerQuest_NPC/FlowerQuestNPC_Head%d.dds"))))
	{
	}
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE0, L"Component_Texture_FlowerQuestNPCBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_FlowerQuest_NPC/FlowerQuestNPC_Body%d.dds"))))
	{
	}
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE0, L"Component_Texture_FlowerQuestNPCHand", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_FlowerQuest_NPC/FlowerQuestNPC_Hand%d.dds"))))
	{
	}
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE0, L"Component_Texture_FlowerQuestNPCFoot", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/3D/Layer_FlowerQuest_NPC/FlowerQuestNPC_Foot%d.dds"))))
	{
	}
#pragma endregion

// 0번 마을

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 0)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage1()
{
	// TODO : 추가하시오
	//숲
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
#pragma region Snail

#pragma region GameObject_Snail
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE1, L"GameObject_Snail", CSnail::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Snail_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE1, L"GameObject_Snail_Impact", CSnail_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SnailBody
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE1, L"Component_Texture_SnailBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SnailBody%d.dds"))))
	{
	}
#pragma endregion

#pragma region Component_Texture_SnailHead
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE1, L"Component_Texture_SnailHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SnailHead%d.dds"))))
	{
	}
#pragma endregion

#pragma endregion

#pragma region Slime

#pragma region GameObject_Slime
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE1, L"GameObject_Slime", CSlime::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Slime_Impact
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE1, L"GameObject_Slime_Impact", CSlime_Impact::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Translucent_Cube
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE1, L"Component_Texture_Slime", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/TranslucentCube%d.dds", 2))))
	{
	}
#pragma endregion

#pragma region GameObject_Slime
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE1, L"GameObject_Crack", CCrack::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Translucent_Cube
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE1, L"Component_Texture_Crack", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/crack%d.png"))))
	{
	}
#pragma endregion


#pragma endregion

#pragma region Stump

#pragma region GameObject_Stump
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE1, L"GameObject_Stump", CStump::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

//#pragma region GameObject_Acorn
//	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Acorn", CAcorn::Create(m_pDevice))))
//		return E_FAIL;
//#pragma endregion

#pragma region Component_Texture_Stump_Head
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE1, L"Component_Texture_Stump_Head", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Head%d.dds"))))
	{
	}
#pragma endregion

#pragma region Component_Texture_Stump_Part
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE1, L"Component_Texture_Stump_Part", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Part%d.dds"))))
	{
	}
#pragma endregion

//#pragma region Component_Texture_Acorn
//	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Acorn", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Head%d.dds"))))
//		return E_FAIL;
//#pragma endregion

//#pragma region Component_Texture_AcornExplosion
//	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_AcornExplosion", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Stump_Head%d.dds"))))
//		return E_FAIL;
//#pragma endregion

#pragma endregion

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE1, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage2()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE2, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage3()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE3, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	if (FAILED(Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"), 3)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage4()
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
		hr = pInstance->Load_Resources_Stage0();
		break;

	case eSCENE_ID::SCENE_STAGE1:
		hr = pInstance->Load_Resources_Stage1();
		break;

	case eSCENE_ID::SCENE_STAGE2:
		hr = pInstance->Load_Resources_Stage2();
		break;

	case eSCENE_ID::SCENE_STAGE3:
		hr = pInstance->Load_Resources_Stage3();
		break;

	case eSCENE_ID::SCENE_STAGE4:
		hr = pInstance->Load_Resources_Stage4();
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


			tTileInfo[iIndex].iOpt = 0;
			tTileInfo[iIndex].iX = iXIndex;
			tTileInfo[iIndex].iZ = iZIndex;



			if (fin.eof())
				break;


			((CTerrainBundle*)pManagement->Get_GameObject(iToScene, _T("Layer_TerrainBundle")))->Set_TerrainInfo(iIndex, iFloor, Temp_Info);


			if (true == bOnOff)
			{
				if (FAILED(pManagement->Add_GameObject_InLayer(iToScene, L"GameObject_CubeTerrain", iToScene, LayerTag, &Temp_Info)))
					return E_FAIL;

			}

		}

		pManagement->Set_TileInfo(tTileInfo, XNumber, ZNumber);


	}

	else
		return E_FAIL;


	fin.close();
	return S_OK;
}
