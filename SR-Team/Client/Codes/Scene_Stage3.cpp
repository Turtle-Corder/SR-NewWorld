#include "stdafx.h"
#include "Player.h"
#include "PreLoader.h"
#include "Scene_Stage0.h"
#include "CubeTerrain.h"
#include "..\Headers\Scene_Stage3.h"

USING(Client)

CScene_Stage3::CScene_Stage3(LPDIRECT3DDEVICE9 _pDeivce)
	: CScene(_pDeivce)
{
}

HRESULT CScene_Stage3::Setup_Scene()
{
	SetWindowText(g_hWnd, L"SCene_Stage3");

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

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_EnergyBolt", SCENE_VOLCANIC, L"Layer_PlayerAtk", &tImpact)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_VOLCANIC, L"GameObject_Snow", SCENE_VOLCANIC, L"Layer_MonsterAtk", &tImpact)))
		return E_FAIL;

	return S_OK;
}

_int CScene_Stage3::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	if (pManagement->Key_Down(VK_F1) && m_pPreLoader->IsFinished())
	{
		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(SCENE_VOLCANIC, L"Layer_Player");
		if (nullptr == pPlayer)
			return -1;

		if (FAILED(pPlayer->Set_ClearInfo(SCENE_VOLCANIC)))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Mouse")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Camera")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Player")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Item")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_MainUI")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Inventory")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Shop")))
			return -1;

		if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Wand")))
			return -1;

		if (FAILED(pManagement->Clear_Except(SCENE_VOLCANIC, SCENE_TOWN)))
		{
			PRINT_LOG(L"Failed To Clear_Except", LOG::CLIENT);
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

	return GAMEOBJECT::NOEVENT;
}

_int CScene_Stage3::LateUpdate_Scene(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CScene_Stage3 * CScene_Stage3::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	return nullptr;
}

void CScene_Stage3::Free()
{
	CScene::Free();
}

HRESULT CScene_Stage3::Setup_Layer_AllObject()
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage3::Setup_Layer_Skybox(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage3::Setup_Layer_Terrain(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage3::Setup_Layer_CubeTerrain(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage3::Setup_Layer_Environment(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage3::Setup_Layer_Monster(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage3::Setup_Layer_Player_Attack(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage3::Setup_Layer_Monster_Attack(const wstring & LayerTag)
{
	return E_NOTIMPL;
}
