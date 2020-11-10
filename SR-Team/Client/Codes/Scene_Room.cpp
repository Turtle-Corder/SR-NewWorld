#include "stdafx.h"
#include "PreLoader.h"
#include "CubeTerrain.h"
#include "Scene_Stage0.h"
#include "Player.h"
#include "SkillSlotManager.h"
#include "Sound_Manager.h"
#include "..\Headers\Scene_Room.h"

USING(Client)

CScene_Room::CScene_Room(LPDIRECT3DDEVICE9 _pDevice)
	: CScene(_pDevice)
{
}

HRESULT CScene_Room::Setup_Scene()
{
	SetWindowText(g_hWnd, L"Csene_Room");


	//--------------------------------------------------
	// 환경
	//--------------------------------------------------
	if (FAILED(Setup_Layer_Skybox(L"Layer_Skybox")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Terrain(L"Layer_Terrain")))
		return E_FAIL;

	//if (FAILED(Setup_Layer_CubeTerrain(L"Layer_CubeTerrain")))
	//	return E_FAIL;

	if (FAILED(Setup_Layer_Environment(L"Layer_Environment")))
		return E_FAIL;

	//--------------------------------------------------
	// 필수 오브젝트
	//--------------------------------------------------
	if (FAILED(Setup_Layer_Mouse(L"Layer_Mouse")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	//--------------------------------------------------
	// UI
	//--------------------------------------------------
	if (FAILED(SetUp_Layer_Item(L"Layer_Item")))
		return E_FAIL;

	if (FAILED(Setup_Layer_UI(L"Layer_MainUI")))
		return E_FAIL;

	if (FAILED(SetUp_Layer_Shop(L"Layer_Shop")))
		return E_FAIL;

	if (FAILED(SetUp_Layer_Inventory(L"Layer_Inventory")))
		return E_FAIL;

	if (FAILED(Setup_Layer_PlayerSkill(L"Layer_PlayerSkill")))
		return E_FAIL;

	// 퀵슬롯에서 사용 가능한 아이템들
	if (FAILED(Setup_Layer_PlayerItem(L"Layer_PlayerItem")))
		return E_FAIL;

	// 메인 퀘스트
	if (FAILED(SetUp_Layer_MainQuest(L"Layer_MainQuest")))
		return E_FAIL;

	m_pPreLoader = CPreLoader::Create(m_pDevice, SCENE_STAGE0);
	if (nullptr == m_pPreLoader)
	{
		PRINT_LOG(L"Failed To PreLoader Create in CScene_Logo", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

_int CScene_Room::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	if (!m_bInit)
	{
		CSoundManager::Get_Instance()->StopSound(CSoundManager::BGM);
		CSoundManager::Get_Instance()->PlayBGM(L"bgm_room.mp3");

		_int iCnt = 0;
		while (true)
		{
			CCubeTerrain* pCubeTerrain = (CCubeTerrain*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_CubeTerrain", iCnt++);
			if (nullptr == pCubeTerrain)	break;

			pCubeTerrain->Set_Active();
		}

		m_bInit = true;
	}

	if (pManagement->Key_Down(VK_F1) && m_pPreLoader->IsFinished())
	{
		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(SCENE_ROOM, L"Layer_Player");
		if (nullptr == pPlayer)
			return -1;

		if (FAILED(pPlayer->Set_ClearInfo(SCENE_TOWN)))
		{
			PRINT_LOG(L"Failed To ClearInfo Refresh", LOG::CLIENT);
			return -1;
		}

		if (FAILED(Travel_NextLayers()))
		{
			PRINT_LOG(L"Failed To Travel Layers in Room", LOG::CLIENT);
			return -1;
		}

		// 무조건 방 -> 마을 단방향 ROOM -> TOWN
		if (FAILED(pManagement->Change_CurrentScene(SCENE_TOWN, CScene_Stage0::Create(m_pDevice))))
		{
			PRINT_LOG(L"Failed To Setup CScene_Room", LOG::CLIENT);
			return -1;
		}

		if (FAILED(pManagement->ClearScene_Component_All(SCENE_ROOM)))
		{
			PRINT_LOG(L"Failed To ClearScene_Component_All in Room", LOG::CLIENT);
			return -1;
		}

		return 1;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CScene_Room::LateUpdate_Scene(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CScene_Room * CScene_Room::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Room* pInstance = new CScene_Room(_pDevice);
	if (FAILED(pInstance->Setup_Scene()))
	{
		PRINT_LOG(L"Failed To Create CScene_Room", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Room::Free()
{
	Safe_Release(m_pPreLoader);

	CScene::Free();
}

HRESULT CScene_Room::Setup_Layer_AllObject()
{
	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_Skybox(const wstring & LayerTag)
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	int TextureID = 2;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ROOM, L"GameObject_Skybox", SCENE_ROOM, LayerTag, &TextureID)))
		return E_FAIL;



	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_Terrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_bool bRefresh = true;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DummyTerrain", SCENE_ROOM, LayerTag, &bRefresh)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_CubeTerrain(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_Environment(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_Camera(const wstring & LayerTag)
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
	tCameraDesc.fFar = 1000.f;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_MainCamera", SCENE_ROOM, LayerTag, &tCameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_UI(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_MainUI", SCENE_ROOM, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Equip", SCENE_ROOM, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Skill", SCENE_ROOM, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillInven", SCENE_ROOM, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_ItemInven", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::SetUp_Layer_Inventory(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Inven", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::SetUp_Layer_Shop(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Shop", SCENE_ROOM, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_ShopChatWnd", SCENE_ROOM, LayerTag)))
		return E_FAIL;


	return S_OK;
}

HRESULT CScene_Room::SetUp_Layer_Item(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Item", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_PlayerSkill(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillSlot_Meteor", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillSlot_IceSpear", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillSlot_ThunderStorm", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillSlot_ManaDrift", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillSlot_Blind", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillSlot_Shield", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillSlot_IceBlast", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_SkillSlot_Explosion", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_PlayerItem(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_ItemSlot_RedPotion", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_ItemSlot_BluePotion", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_ItemSlot_RedElixir", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_ItemSlot_BlueElixir", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::SetUp_Layer_MainQuest(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_MainQuest", SCENE_ROOM, LayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_NpcWnd", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::Travel_NextLayers()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_Mouse")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_Player")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_Item")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_MainUI")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_Inventory")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_Shop")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_Wand")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_PlayerSkill")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_PlayerItem")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ROOM, L"Layer_MainQuest")))
		return E_FAIL;

	if (FAILED(pManagement->Clear_Except(SCENE_ROOM, SCENE_TOWN)))
	{
		PRINT_LOG(L"Failed To Clear_Except in Room", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}


HRESULT CScene_Room::Setup_Layer_Mouse(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Mouse", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Room::Setup_Layer_Player(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Player", SCENE_ROOM, LayerTag)))
		return E_FAIL;

	return S_OK;
}
