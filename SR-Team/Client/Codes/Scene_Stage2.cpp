#include "stdafx.h"
#include "PreLoader.h"
#include "Player.h"
#include "CubeTerrain.h"
#include "Scene_Stage0.h"
#include "..\Headers\Scene_Stage2.h"

USING(Client)

CScene_Stage2::CScene_Stage2(LPDIRECT3DDEVICE9 _pDeivce)
	: CScene(_pDeivce)
{
}

HRESULT CScene_Stage2::Setup_Scene()
{
	SetWindowText(g_hWnd, L"SCene_Stage2");

	if (FAILED(Setup_Layer_Terrain(L"Layer_Terrain")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	ZeroMemory(&tImpact, sizeof(INSTANTIMPACT));
	tImpact.vPosition = { 999.f, 999.f, 999.f };

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_EnergyBolt", SCENE_ICELAND, L"Layer_PlayerAtk", &tImpact)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Snow", SCENE_ICELAND, L"Layer_MonsterAtk", &tImpact)))
		return E_FAIL;

	return S_OK;
}

_int CScene_Stage2::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	if (pManagement->Key_Down(VK_F1) && m_pPreLoader->IsFinished())
	{
		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(SCENE_ICELAND, L"Layer_Player");
		if (nullptr == pPlayer)
			return -1;

		if (FAILED(pPlayer->Set_ClearInfo(SCENE_ICELAND)))
			return -1;

		if (FAILED(Travel_NextLayers()))
		{
			PRINT_LOG(L"Failed To Travel Layers in Iceland", LOG::CLIENT);
			return -1;
		}

		_int iCnt = 0;
		while (true)
		{
			CCubeTerrain* pCubeTerrain = (CCubeTerrain*)pManagement->Get_GameObject(SCENE_TOWN, L"Layer_CubeTerrain", iCnt++);
			if (nullptr == pCubeTerrain)	break;

			pCubeTerrain->SetActive();
		}

		// 얼음 -> 마을 ICELAND -> TOWN 단방향
		if (FAILED(pManagement->Change_CurrentScene(SCENE_TOWN, CScene_Stage0::Create(m_pDevice))))
		{
			PRINT_LOG(L"Failed To Setup CScene_Room", LOG::CLIENT);
			return -1;
		}

		return 1;
	}
	//--------------------------------------------------
	// TODO : 스테이지 클리어 조건
	//--------------------------------------------------

	return GAMEOBJECT::NOEVENT;
}

_int CScene_Stage2::LateUpdate_Scene(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CScene_Stage2 * CScene_Stage2::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Stage2* pInstance = new CScene_Stage2(_pDevice);
	if (FAILED(pInstance->Setup_Scene()))
	{
		PRINT_LOG(L"Failed To Create CScene_Stage2", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage2::Free()
{

	CScene::Free();
}

HRESULT CScene_Stage2::Setup_Layer_AllObject()
{
	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_Skybox(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_Terrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DummyTerrain", SCENE_STAGE2, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_CubeTerrain(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_Environment(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_Monster(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_Player_Attack(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_Monster_Attack(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage2::Travel_NextLayers()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Mouse")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Player")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Item")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_MainUI")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Inventory")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Shop")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Wand")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_PlayerSkill")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_PlayerItem")))
		return E_FAIL;

	if (FAILED(pManagement->Clear_Except(SCENE_ICELAND, SCENE_TOWN)))
	{
		PRINT_LOG(L"Failed To Clear_Except", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}
