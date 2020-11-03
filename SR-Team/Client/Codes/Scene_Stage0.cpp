#include "stdafx.h"
#include "PreLoader.h"
#include "TerrainBundle.h"
#include "..\Headers\Scene_Stage0.h"

USING(Client)

CScene_Stage0::CScene_Stage0(LPDIRECT3DDEVICE9 _pDevice)
	: CScene(_pDevice)
{
}

HRESULT CScene_Stage0::Setup_Scene()
{
	SetWindowText(g_hWnd, L"CScene_Stage0");

	//--------------------------------------------------
	// ȯ��
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
	// �ʼ� ������Ʈ
	//--------------------------------------------------
	if (FAILED(Setup_Layer_Mouse(L"Layer_Mouse")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Player(L"Layer_Player")))
		return E_FAIL;


	//--------------------------------------------------
	// ���� & ����
	//--------------------------------------------------
	if (FAILED(Setup_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Monster_Attack(L"Layer_MonsterAtk")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Player_Attack(L"Layer_PlayerAtk")))
		return E_FAIL;


	//--------------------------------------------------
	// UI
	//--------------------------------------------------
	if (FAILED(SetUp_Layer_Item(L"Layer_Item")))
		return E_FAIL;

	if (FAILED(Setup_Layer_UI(L"Layer_MainUI")))
		return E_FAIL;

	if (FAILED(SetUp_Layer_Inventory(L"Layer_Inventory")))
		return E_FAIL;

	if (FAILED(SetUp_Layer_Shop(L"Layer_Shop")))
		return E_FAIL;


	return S_OK;
}

_int CScene_Stage0::Update_Scene(_float _fDeltaTime)
{
	//--------------------------------------------------
	// TODO : �������� Ŭ���� ����
	//--------------------------------------------------

	return 0;
}

_int CScene_Stage0::LateUpdate_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	// Src�� ������ Dst�� �ǰ���
	//if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_STAGE0, L"Layer_MonsterAtk", L"Layer_Player", L"Com_Collider", L"Com_DmgInfo")))
	//	return -1;

	//if (FAILED(pManagement->CollisionSphere_Detection_Layers(SCENE_STAGE0, L"Layer_PlayerAtk" , L"Layer_Monster", L"Com_Collider", L"Com_DmgInfo")))
	//	return -1;

	return 0;
}

CScene_Stage0 * CScene_Stage0::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Stage0* pInstance = new CScene_Stage0(_pDevice);
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
		//����
		TCHAR szGameObjectName[MAX_PATH] = L"";
		TCHAR szLayerName[MAX_PATH] = L"";
		TCHAR szFloor[MAX_PATH] = L"";
		TCHAR szXPos[MAX_PATH] = L"";
		TCHAR szZPos[MAX_PATH] = L"";


		while (true)
		{
			//ADD_TO_LAYER ���� �ޱ�
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

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Terrain", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Camera(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera::CAMERA_DESC tCameraDesc;
	ZeroMemory(&tCameraDesc, sizeof(CCamera::CAMERA_DESC));
	D3DXMatrixIdentity(&tCameraDesc.matView);
	tCameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	D3DXMatrixIdentity(&tCameraDesc.matProj);
	tCameraDesc.fFovY = D3DXToRadian(60.f);
	tCameraDesc.fAspect = (float)WINCX / WINCY;
	tCameraDesc.fNear = 1.f;
	tCameraDesc.fFar = 100.f;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_MainCamera", SCENE_STAGE0, LayerTag, &tCameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Player(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Player", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Monster(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Snail", SCENE_STAGE0, LayerTag , &_vec3(10.f , 0.f , 5.f))))/*���� StartPos*/
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Monster_Attack(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// ���̾� ��������
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Snail_Impact", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Player_Attack(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// ���̾� ��������
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Meteor", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_UI(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_MainUI", SCENE_STAGE0, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Equip", SCENE_STAGE0, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Skill", SCENE_STAGE0, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillInven", SCENE_STAGE0, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_ItemInven", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::SetUp_Layer_Inventory(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Inven", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::SetUp_Layer_Shop(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Shop", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::SetUp_Layer_Item(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Item", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}


HRESULT CScene_Stage0::Setup_Layer_Mouse(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Mouse", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Environment(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	vector<void*> Test;


	_vec3 Pos = { 20.f, 0.f, 20.f };
	Test.emplace_back(&Pos);	
	Pos = { 35.f, 0.f, 35.f };

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Flower", SCENE_STAGE0, LayerTag, &Test)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_CubeTerrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//�ε�����

	wifstream fin;
	wstring wstrFilePath = _T("../DataPath/MapSource/Stage2.txt");
	fin.open(wstrFilePath.c_str());
	if (!fin.fail())
	{
		//����
		TCHAR szXVerCount[MAX_PATH] = L"";
		TCHAR szZVerCount[MAX_PATH] = L"";
		TCHAR szFloor[MAX_PATH] = L"";
		TCHAR szIndex[MAX_PATH] = L"";
		TCHAR szOnOff[MAX_PATH] = L"";
		TCHAR szTextureID[MAX_PATH] = L"";
		TCHAR szMovePossible[10] = L"";

		//X, Z �హ��
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


			//Cube Info �ޱ�
			fin.getline(szFloor, MAX_PATH, L'|');
			fin.getline(szIndex, MAX_PATH, L'|');
			fin.getline(szOnOff, MAX_PATH, L'|');
			fin.getline(szTextureID, MAX_PATH, L'|');
			fin.getline(szMovePossible, MAX_PATH);
			_uint iFloor = _ttoi(szFloor);
			_uint iIndex = _ttoi(szIndex);
			_bool bOnOff = 1 && (_ttoi(szOnOff));
			_uint iTextureID = _ttoi(szTextureID);

			//Index ���ϱ�
			_uint iXIndex = iIndex % XNumber;
			_uint iZIndex = iIndex / XNumber;

			//�Ѱ��� TerrainInfo
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
				if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_CubeTerrain", SCENE_STAGE0, LayerTag, &Temp_Info)))
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