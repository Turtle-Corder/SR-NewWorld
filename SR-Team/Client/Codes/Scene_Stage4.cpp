#include "stdafx.h"
#include "Player.h"
#include "PreLoader.h"
#include "Scene_Stage0.h"
#include "CubeTerrain.h"
#include "Sound_Manager.h"
#include "..\Headers\Scene_Stage4.h"

USING(Client)

CScene_Stage4::CScene_Stage4(LPDIRECT3DDEVICE9 _pDeivce)
	: CScene(_pDeivce)
{
}

HRESULT CScene_Stage4::Setup_Scene()
{
	SetWindowText(g_hWnd, L"CScene_Stage4");

	CManagement* pManagement = CManagement::Get_Instance();

	bool Ending = TRUE;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Title", SCENE_LAB, L"Layer_Background", &Ending)))
		return E_FAIL;


	return S_OK;
}

_int CScene_Stage4::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	if (!m_bInit)
	{
		CSoundManager::Get_Instance()->StopSound(CSoundManager::BGM);
		CSoundManager::Get_Instance()->PlayBGM(L"bgm_volcanic.mp3");


		m_bTravel = false;
		m_bInit = true;
	}

	if (pManagement->Key_Down(VK_RETURN))
		return 999;


	return GAMEOBJECT::NOEVENT;
}

_int CScene_Stage4::LateUpdate_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

HRESULT CScene_Stage4::Set_SceneEvent(_int _iEventNo)
{
	return S_OK;
}

CScene_Stage4 * CScene_Stage4::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Stage4* pInstance = new CScene_Stage4(_pDevice);
	if (FAILED(pInstance->Setup_Scene()))
	{
		PRINT_LOG(L"Failed To Create CScene_Stage4", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage4::Free()
{
	CScene::Free();
}
