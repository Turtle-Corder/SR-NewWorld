#include "stdafx.h"
#include "PreLoader.h"
#include "Player.h"
#include "TerrainBundle.h"
#include "Scene_Stage1.h"
#include "CubeTerrain.h"
#include "..\Headers\Scene_Stage0.h"

USING(Client)

CScene_Stage0::CScene_Stage0(LPDIRECT3DDEVICE9 _pDevice, _bool _bReload)
	: CScene(_pDevice)
	, m_bReload(_bReload)
{
}

HRESULT CScene_Stage0::Setup_Scene()
{
	SetWindowText(g_hWnd, L"CScene_Stage0");

	//--------------------------------------------------
	// 환경
	//--------------------------------------------------
	//if (FAILED(Setup_Layer_Skybox(L"Layer_Skybox")))
	//	return E_FAIL;

	if (FAILED(Setup_Layer_Terrain(L"Layer_Terrain")))
		return E_FAIL;

	//if (FAILED(Setup_Layer_CubeTerrain(L"Layer_CubeTerrain")))
	//	return E_FAIL;

	if (FAILED(Setup_Layer_Environment(L"Layer_Environment")))
		return E_FAIL;


	//--------------------------------------------------
	// 꽃 퀘스트
	//--------------------------------------------------
	if (FAILED(Setup_Layer_FlowerQuest(L"Layer_FlowerQuest")))
		return E_FAIL;

	if (FAILED(Setup_Layer_FlowerQuestNPC(L"Layer_FlowerQuestNPC")))
		return E_FAIL;

	eSCENE_ID ePreLoadScene = SCENE_STAGE1;

	if (m_bReload)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player");

		// 다 끝나고 마을로 돌아오면 안됨!
		ePreLoadScene = (eSCENE_ID)pPlayer->Get_ClearInfo();
	}

	m_pPreLoader = CPreLoader::Create(m_pDevice, ePreLoadScene);
	if (nullptr == m_pPreLoader)
	{
		PRINT_LOG(L"Failed To PreLoader Create in CScene_Stage0", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

_int CScene_Stage0::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	if (pManagement->Key_Down(VK_F2) && m_pPreLoader->IsFinished())
	{
		if (FAILED(pManagement->Change_CurrentScene(SCENE_STAGE1, CScene_Stage1::Create(m_pDevice))))
		{
			PRINT_LOG(L"Failed To Setup CScene_Stage1", LOG::CLIENT);
			return -1;
		}

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_STAGE0, L"Layer_Mouse")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_STAGE0, L"Layer_Camera")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_STAGE0, L"Layer_Player")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_STAGE0, L"Layer_Item")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_STAGE0, L"Layer_MainUI")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_STAGE0, L"Layer_Inventory")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_STAGE0, L"Layer_Shop")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_STAGE0, L"Layer_Wand")))
			return -1;

		if (FAILED(pManagement->Clear_Except(SCENE_STAGE0, SCENE_STAGE1)))
		{
			PRINT_LOG(L"Failed To Clear_Except", LOG::CLIENT);
			return -1;
		}

		_int iCnt = 0;
		while (true)
		{
			CCubeTerrain* pCubeTerrain = (CCubeTerrain*)pManagement->Get_GameObject(SCENE_STAGE1, L"Layer_CubeTerrain", iCnt++);
			if (nullptr == pCubeTerrain)	break;

			pCubeTerrain->SetActive();
		}

		return 1;
	}

	return 0;
}

_int CScene_Stage0::LateUpdate_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	// Src가 공격자 Dst가 피격자
	//if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_STAGE0, L"Layer_MonsterAtk", L"Layer_Player", L"Com_Collider", L"Com_DmgInfo")))
	//	return -1;

	//if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_STAGE0, L"Layer_PlayerAtk" , L"Layer_Monster", L"Com_Collider", L"Com_DmgInfo")))
	//	return -1;

	//if (FAILED(pManagement->CollisionSphere_Impulse_Layers(SCENE_STAGE0, L"Layer_PlayerAtk", L"Layer_Monster", L"Com_Collider", L"Com_DmgInfo")))
	//	return -1;

	//if(FAILED(pManagement->CollisionSphere_Impulse_Layers(SCENE_STAGE0, L"Layer_Monster", L"Layer_Monster", L"Com_Collider", L"")))
		//몬스터들의 Base를 Transform이름을 통일하게 수작업할것

	//pManagement->CollisionBox_Detection_Layers(Src , Dst) Src -> Dst를 공격
	//pManagement->CollisionBox_Detection_Layers_Both() // 서로공격
	return 0;
}

CScene_Stage0 * CScene_Stage0::Create(LPDIRECT3DDEVICE9 _pDevice, _bool _bReload)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Stage0* pInstance = new CScene_Stage0(_pDevice, _bReload);
	if (FAILED(pInstance->Setup_Scene()))
	{
		PRINT_LOG(L"Failed To Create CScene_Stage0", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage0::Free()
{
	Safe_Release(m_pPreLoader);

	CScene::Free();
}

HRESULT CScene_Stage0::Setup_Layer_AllObject()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	wifstream fin;
	wstring wstrFilePath = _T("../Resources/TestClientObject.txt");
	fin.open(wstrFilePath.c_str());
	if (!fin.fail())
	{
		//변수
		TCHAR szGameObjectName[MAX_PATH] = L"";
		TCHAR szLayerName[MAX_PATH] = L"";
		TCHAR szFloor[MAX_PATH] = L"";
		TCHAR szXPos[MAX_PATH] = L"";
		TCHAR szZPos[MAX_PATH] = L"";


		while (true)
		{
			//ADD_TO_LAYER 변수 받기
			fin.getline(szGameObjectName, MAX_PATH, L'|');
			fin.getline(szLayerName, MAX_PATH, L'|');
			fin.getline(szFloor, MAX_PATH, L'|');
			fin.getline(szXPos, MAX_PATH, L'|');
			fin.getline(szZPos, MAX_PATH, L'|');
			
			_float X, Y, Z;

			X = (_float)_ttof(szXPos);
			Y = (_float)_ttof(szFloor);
			Z = (_float)_ttof(szZPos);


			_vec3 vPosition = { X, Y, Z };

			if (fin.eof())
				break;

			vector<void*> test;

			test.emplace_back(&vPosition);

			if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, szGameObjectName, SCENE_STAGE0, szLayerName, &test)))
					return E_FAIL;


		}
	}

	else
		return E_FAIL;

	fin.close();


	return S_OK;
}


HRESULT CScene_Stage0::Setup_Layer_Skybox(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Skybox", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Terrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DummyTerrain", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Environment(const wstring & LayerTag)
{
	//CManagement* pManagement = CManagement::Get_Instance();
	//if (nullptr == pManagement)
	//	return E_FAIL;

	//vector<void*> Test;


	//_vec3 Pos = { 20.f, 0.f, 20.f };
	//Test.emplace_back(&Pos);	
	//Pos = { 35.f, 0.f, 35.f };

	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Flower", SCENE_STAGE0, LayerTag, &Test)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_FlowerQuest(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_FlowerQuest", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_FlowerQuestNPC(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_FlowerQuestNPC", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_CubeTerrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//로딩파츠

	wifstream fin;
	wstring wstrFilePath = _T("../DataPath/MapSource/Stage2.txt");
	fin.open(wstrFilePath.c_str());
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
		if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_TerrainBundle", SCENE_STAGE0, _T("Layer_TerrainBundle"), &iFloorMax)))
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


			((CTerrainBundle*)pManagement->Get_GameObject(SCENE_STAGE0, _T("Layer_TerrainBundle")))->Set_TerrainInfo(iIndex, iFloor, Temp_Info);


			if (true == bOnOff)
			{
				if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_CubeTerrain", SCENE_STAGE0, LayerTag, &Temp_Info)))
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
