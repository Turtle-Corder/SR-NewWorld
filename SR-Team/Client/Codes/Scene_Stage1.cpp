#include "stdafx.h"
#include "PreLoader.h"
#include "..\Headers\Scene_Stage1.h"

USING(Client)

CScene_Stage1::CScene_Stage1(LPDIRECT3DDEVICE9 _pDevice)
	: CScene(_pDevice)
{
}

HRESULT CScene_Stage1::Setup_Scene()
{
	SetWindowText(g_hWnd, L"CScene_Stage1");

	if (FAILED(Setup_Layer_Terrain(L"Layer_Terrain")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_EnergyBolt", SCENE_STAGE1, L"Layer_PlayerAtk")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime_Impact", SCENE_STAGE1, L"Layer_MonsterAtk")))
		return E_FAIL;
	
	return S_OK; 
}

_int CScene_Stage1::Update_Scene(_float _fDeltaTime)
{
	//--------------------------------------------------
	// TODO : 스테이지 클리어 조건
	//--------------------------------------------------

	return GAMEOBJECT::NOEVENT;
}

_int CScene_Stage1::LateUpdate_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	// Src가 공격자 Dst가 피격자
	if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_STAGE1, L"Layer_MonsterAtk", L"Layer_Player", L"Com_Collider", L"Com_DmgInfo")))
		return -1;

	if (FAILED(pManagement->CollisionSphere_Detection_Layers(SCENE_STAGE1, L"Layer_PlayerAtk" , L"Layer_Monster", L"Com_Collider", L"Com_DmgInfo")))
		return -1;

	return GAMEOBJECT::NOEVENT;
}

CScene_Stage1 * CScene_Stage1::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Stage1* pInstance = new CScene_Stage1(_pDevice);
	if (FAILED(pInstance->Setup_Scene()))
	{
		PRINT_LOG(L"Failed To Create CScene_Stage0", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage1::Free()
{
	Safe_Release(m_pPreLoader);

	CScene::Free();
}

HRESULT CScene_Stage1::Setup_Layer_AllObject()
{
	return S_OK;
}

HRESULT CScene_Stage1::Setup_Layer_Skybox(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage1::Setup_Layer_Terrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DummyTerrain", SCENE_STAGE1, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage1::Setup_Layer_CubeTerrain(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage1::Setup_Layer_Environment(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage1::Setup_Layer_Monster(const wstring & LayerTag)
{
	/*
Slime||0|12.48|33.08|
Slime||0|7.68|36.88|
Slime||0|12|40.96|
Slime||0|18.4|38.2|
Slime||0|17.08|32.52|
Slime||0|13.44|28.64|
Wolf||0|63.44|17.68|
Wolf||0|66.04|25.04|
Wolf||0|55.04|25.24|
Wolf||0|55.6|18.88|
Wolf||0|61.72|29.76|
Wolf||0|68.24|20.56|

	*/

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	SLIMEINFO tSlimInfo;
	ZeroMemory(&tSlimInfo, sizeof(SLIMEINFO));

	tSlimInfo.iCurCount = 1;
	tSlimInfo.vPos = {12.48f, 0.f, 33.08f };

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag, &tSlimInfo)))
		return E_FAIL;

	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag)))
	//	return E_FAIL;

	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag)))
	//	return E_FAIL;

	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag)))
	//	return E_FAIL;

	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag)))
	//	return E_FAIL;

	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag)))
	//	return E_FAIL;



	return S_OK;
}

HRESULT CScene_Stage1::Setup_Layer_Player_Attack(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage1::Setup_Layer_Monster_Attack(const wstring & LayerTag)
{
	return S_OK;
}
