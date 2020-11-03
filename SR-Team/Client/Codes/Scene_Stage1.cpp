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
	//if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_STAGE1, L"Layer_MonsterAtk", L"Layer_Player", L"Com_Collider", L"Com_DmgInfo")))
	//	return -1;

	//if (FAILED(pManagement->CollisionSphere_Detection_Layers(SCENE_STAGE1, L"Layer_PlayerAtk" , L"Layer_Monster", L"Com_Collider", L"Com_DmgInfo")))
	//	return -1;

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

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DummyTerrain", SCENE_ROOM, LayerTag)))
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
