#include "stdafx.h"
#include "PreLoader.h"
#include "CubeTerrain.h"
#include "Player.h"
#include "Scene_Stage0.h"
#include "Sound_Manager.h"
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

	if (FAILED(Setup_Layer_Skybox(L"Layer_Skybox")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Projectile()))
		return E_FAIL;
	
	if (FAILED(Setup_Layer_ActiveObject(L"Layer_Active")))
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

	if (!m_bInit)
	{
		CSoundManager::Get_Instance()->StopSound(CSoundManager::BGM);
		CSoundManager::Get_Instance()->PlayBGM(L"bgm_room.mp3");

		if (FAILED(Respawn_Palyer()))
			return GAMEOBJECT::WARN;

		_int iCnt = 0;
		while (true)
		{
			CCubeTerrain* pCubeTerrain = (CCubeTerrain*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_CubeTerrain", iCnt++);
			if (nullptr == pCubeTerrain)	break;

			pCubeTerrain->Set_Active();
		}

		m_bTravel = false;
		m_bInit = true;
	}

	if ((pManagement->Key_Down(VK_F1) || (m_bClear && m_bTravel)) && m_pPreLoader->IsFinished())
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


	if (!m_bBossInit)
	{
		CGameObject* pClearCheck = pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Monster");
		if (nullptr == pClearCheck)
		{
			_vec3 vPos = { 58.f, 0.f, 20.f };

			pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Stump", pManagement->Get_CurrentSceneID(), L"Layer_Monster", &vPos);
			m_bBossInit = true;
		}
	}

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

	if (FAILED(pManagement->CollisionSphere_Detection_Layers(SCENE_STAGE1, L"Layer_Player", L"Layer_Active", L"Com_Collider", L"Com_DmgInfo")))
		return -1;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CScene_Stage1::Set_SceneEvent(_int _iEventNo)
{
	switch ((eSceneEventID)_iEventNo)
	{
	case eSceneEventID::EVENT_RESET:
		m_bTravel = false;
		break;

	case eSceneEventID::EVENT_CLEAR:
		m_pTravelTrigger->Set_Active();
		m_bClear = true;
		break;

	case eSceneEventID::EVNET_TRAVEL:
		m_bTravel = true;
		break;

	default:
		break;
	}

	return S_OK;
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


	
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;
		int TextureID = 5;

		if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Skybox", SCENE_STAGE1, LayerTag, &TextureID)))
			return E_FAIL;


	


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

//Slime||0|12.48|33.08|
//Slime||0|7.68|36.88|
//Slime||0|12|40.96|
//Slime||0|18.4|38.2|
//Slime||0|17.08|32.52|
//Slime||0|13.44|28.64|
//Wolf||0|63.44|17.68|
//Wolf||0|66.04|25.04|
//Wolf||0|55.04|25.24|
//Wolf||0|55.6|18.88|
//Wolf||0|61.72|29.76|
//Wolf||0|68.24|20.56|


	//1 || 0 | 59.72 | 15.88 |
	//	1 || 0 | 53.56 | 19.76 |
	//	1 || 0 | 57.92 | 25.16 |



	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	SLIMEINFO tSlimInfo;
	ZeroMemory(&tSlimInfo, sizeof(SLIMEINFO));
	tSlimInfo.iTextureNumber = 0;
	tSlimInfo.vPos = { 14.f, 0.f, 32.32f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Snail", SCENE_STAGE1, LayerTag, &tSlimInfo)))
		return E_FAIL;


	tSlimInfo.iTextureNumber = 1;
	tSlimInfo.vPos = { 53.56f, 0.f, 19.76f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Snail", SCENE_STAGE1, LayerTag, &tSlimInfo)))
		return E_FAIL;

	tSlimInfo.iTextureNumber = 2;
	tSlimInfo.vPos = { 59.72f, 0.f, 15.88f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Snail", SCENE_STAGE1, LayerTag, &tSlimInfo)))
		return E_FAIL;

	//tSlimInfo.iTextureNumber = 1;
	//tSlimInfo.vPos = { 32.f, 0.f, 40.96f };
	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Snail", SCENE_STAGE1, LayerTag, &tSlimInfo)))
	//	return E_FAIL;

	//tSlimInfo.iTextureNumber = 2;
	//tSlimInfo.vPos = { 18.4f, 0.f, 38.2f };
	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Snail", SCENE_STAGE1, LayerTag, &tSlimInfo)))
	//	return E_FAIL;

	//vPos = { 13.44f, 0.f, 28.64f };
	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Snail", SCENE_STAGE1, LayerTag, &vPos)))
	//	return E_FAIL;

	//vPos = { 17.08f, 0.f, 32.52f };
	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Snail", SCENE_STAGE1, LayerTag, &vPos)))
	//	return E_FAIL;

	//----------------------------------------------------------------------------------------------------


	//	1 || 0 | 14 | 32.32 |
	//	1 || 0 | 8.88 | 39.4 |
	//	1 || 0 | 14.28 | 38.28 |
	//	1 || 0 | 18.72 | 35.4 |
	//	1 || 0 | 12.16 | 43.36 |



	tSlimInfo.iCurCount = 1;
	tSlimInfo.iTextureNumber = 0;
	tSlimInfo.vPos = { 8.88f, 0.f, 39.4f };
	
	tSlimInfo.iCurCount = 1;
	tSlimInfo.iTextureNumber = 0;
	tSlimInfo.vPos = { 12.48f, 0.f, 33.08f };
	
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag, &tSlimInfo)))
		return E_FAIL;
	
	tSlimInfo.vPos = { 66.04f, 0.f, 25.04f };
	tSlimInfo.iTextureNumber = 2;
	
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag, &tSlimInfo)))
		return E_FAIL;
	
	tSlimInfo.iTextureNumber = 4;
	tSlimInfo.vPos = { 55.04f, 0.f, 25.24f };
	
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag, &tSlimInfo)))
		return E_FAIL;

	////tSlimInfo.vPos = { 55.6f, 0.f, 18.88f };

	////if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag, &tSlimInfo)))
	////	return E_FAIL;

	//tSlimInfo.vPos = { 61.72f, 0.f, 29.76f };

	//if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag, &tSlimInfo)))
	//	return E_FAIL;

	////tSlimInfo.vPos = { 68.24f, 0.f, 20.56f };

	////if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE1, L"GameObject_Slime", SCENE_STAGE1, LayerTag, &tSlimInfo)))
	////	return E_FAIL;

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

HRESULT CScene_Stage1::Setup_Layer_ActiveObject(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	EVENT_INFO tEventInfo;
	tEventInfo.iEventNo = eSceneEventID::EVNET_TRAVEL;
	tEventInfo.vSpawnPos = { 65.f, 0.f, 20.f };
	tEventInfo.iFloatOption = 1;

	if (FAILED(pManagement->Add_GameObject_InLayer(&m_pTravelTrigger, SCENE_STATIC, L"GameObject_Trigger", SCENE_FOREST, LayerTag, &tEventInfo)))
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

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Shop")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_FOREST, L"Layer_Inventory")))
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

HRESULT CScene_Stage1::Respawn_Palyer()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pTransformHead = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");
	CTransform* pTransformBody = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform1");
	if (nullptr == pTransformHead || nullptr == pTransformBody)	return E_FAIL;

	_vec3 vSpawnPos = { 3.f, pTransformHead->Get_Desc().vPosition.y, 3.f };
	pTransformHead->Set_Position(vSpawnPos);

	vSpawnPos = { 3.f, pTransformBody->Get_Desc().vPosition.y, 3.f };
	pTransformBody->Set_Position(vSpawnPos);

	return S_OK;
}
