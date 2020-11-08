#include "stdafx.h"
#include "PreLoader.h"
#include "CubeTerrain.h"
#include "Player.h"
#include "Scene_Stage0.h"
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

	if (FAILED(Setup_Layer_NPC(L"Layer_NPC")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Projectile()))
		return E_FAIL;
	
	m_pPreLoader = CPreLoader::Create(m_pDevice, SCENE_TOWN);
	if (nullptr == m_pPreLoader)
	{
		PRINT_LOG(L"Failed To PreLoader Create in CScene_Stage1", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK; 
}

_int CScene_Stage1::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	if (pManagement->Key_Down(VK_F1) && m_pPreLoader->IsFinished())
	{
		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(SCENE_FOREST, L"Layer_Player");
		if (nullptr == pPlayer)
			return -1;

		if (FAILED(pPlayer->Set_ClearInfo(SCENE_FOREST)))
			return -1;

		if (FAILED(Travel_NextLayers()))
		{
			PRINT_LOG(L"Failed To Travel Layers in Forest", LOG::CLIENT);
			return -1;
		}

		_int iCnt = 0;
		while (true)
		{
			CCubeTerrain* pCubeTerrain = (CCubeTerrain*)pManagement->Get_GameObject(SCENE_TOWN, L"Layer_CubeTerrain", iCnt++);
			if (nullptr == pCubeTerrain)	break;

			pCubeTerrain->Set_Active();
		}

		// 숲 -> 마을 FOREST -> TOWN 단방향
		if (FAILED(pManagement->Change_CurrentScene(SCENE_TOWN, CScene_Stage0::Create(m_pDevice))))
		{
			PRINT_LOG(L"Failed To Setup CScene_Room", LOG::CLIENT);
			return -1;
		}

		if (FAILED(pManagement->ClearScene_Component_All(SCENE_FOREST)))
		{
			PRINT_LOG(L"Failed To ClearScene_Component_All in Forest", LOG::CLIENT);
			return -1;
		}

		return 1;
	}
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

	if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_STAGE1, L"Layer_PlayerAtk" , L"Layer_Monster", L"Com_Collider", L"Com_DmgInfo")))
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
		PRINT_LOG(L"Failed To Create CScene_Stage1", LOG::CLIENT);
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

	_vec3 vSpawnPos = { 7.68f, 0.f, 36.88f };

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Stump", SCENE_STAGE1, LayerTag, &vSpawnPos)))
		return E_FAIL;

	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Snail", SCENE_STAGE1, LayerTag, &vSpawnPos)))
	//	return E_FAIL;

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

HRESULT CScene_Stage1::Setup_Layer_Projectile()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	ZeroMemory(&tImpact, sizeof(INSTANTIMPACT));
	tImpact.vPosition = { 999.f, 999.f, 999.f };

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_EnergyBolt", SCENE_FOREST, L"Layer_PlayerAtk", &tImpact)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_FOREST, L"GameObject_Slime_Impact", SCENE_FOREST, L"Layer_MonsterAtk", &tImpact)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CScene_Stage1::Setup_Layer_NPC(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Stage1NPC", SCENE_STAGE1, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage1::Travel_NextLayers()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Mouse")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Player")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Item")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_MainUI")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Inventory")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Shop")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Wand")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_PlayerSkill")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_PlayerItem")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_MainQuest")))
		return E_FAIL;

	if (FAILED(pManagement->Clear_Except(SCENE_FOREST, SCENE_TOWN)))
	{
		PRINT_LOG(L"Failed To Clear_Except", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}
