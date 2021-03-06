#include "stdafx.h"
#include "PreLoader.h"
#include "Player.h"
#include "CubeTerrain.h"
#include "Scene_Stage0.h"
#include "Sound_Manager.h"
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

	if (FAILED(Setup_Layer_Projectile()))
		return E_FAIL;

	if (FAILED(Setup_Layer_ActiveObject(L"Layer_Active")))
		return E_FAIL;

	if (FAILED(Setup_Layer_NPC(L"Layer_NPC")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Skybox(L"Layer_Skybox")))
		return E_FAIL;

	if (FAILED(SetUp_Layer_IceLandQuest(L"Layer_IceLandQuest")))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_pPreLoader = CPreLoader::Create(m_pDevice, SCENE_TOWN);
	if (nullptr == m_pPreLoader)
	{
		PRINT_LOG(L"Failed To PreLoader Create in CScene_Stage2", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

_int CScene_Stage2::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	if (!m_bInit)
	{
		CSoundManager::Get_Instance()->StopSound(CSoundManager::BGM);
		CSoundManager::Get_Instance()->PlayBGM(L"bgm_iceland.mp3");

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

		// 얼음 -> 마을 ICELAND -> TOWN 단방향
		if (FAILED(pManagement->Change_CurrentScene(SCENE_TOWN, CScene_Stage0::Create(m_pDevice))))
		{
			PRINT_LOG(L"Failed To Setup CScene_Room", LOG::CLIENT);
			return -1;
		}

		if (FAILED(pManagement->ClearScene_Component_All(SCENE_ICELAND)))
		{
			PRINT_LOG(L"Failed To ClearScene_Component_All in Iceland", LOG::CLIENT);
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
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	// Src가 공격자 Dst가 피격자
	if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_ICELAND, L"Layer_MonsterAtk", L"Layer_Player", L"Com_Collider", L"Com_DmgInfo")))
		return -1;

	if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_ICELAND, L"Layer_PlayerAtk", L"Layer_Monster", L"Com_Collider", L"Com_DmgInfo")))
		return -1;

	if (FAILED(pManagement->CollisionSphere_Detection_Layers(SCENE_ICELAND, L"Layer_Player", L"Layer_Active", L"Com_Collider", L"Com_DmgInfo")))
		return -1;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CScene_Stage2::Set_SceneEvent(_int _iEventNo)
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
	Safe_Release(m_pPreLoader);

	CScene::Free();
}

HRESULT CScene_Stage2::Setup_Layer_AllObject()
{
	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_Skybox(const wstring & LayerTag)
{


	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	int TextureID = 3;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE2, L"GameObject_Skybox", SCENE_STAGE2, LayerTag, &TextureID)))
		return E_FAIL;


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
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vSpawnPos = { 20.f, 0.f, 30.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	vSpawnPos = { 26.f, 0.f, 26.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	vSpawnPos = { 27.f, 0.f, 31.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Yeti", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	//----------------------------------------------------------------------------------------------------

	vSpawnPos = { 42.f, 0.f, 22.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	vSpawnPos = { 44.f, 0.f, 22.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Yeti", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	//----------------------------------------------------------------------------------------------------

	vSpawnPos = { 65.f, 0.f, 23.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	vSpawnPos = { 65.f, 0.f, 26.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Yeti", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	//----------------------------------------------------------------------------------------------------

	vSpawnPos = { 62.f, 0.f, 37.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	vSpawnPos = { 66.f, 0.f, 37.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;


	vSpawnPos = { 60.f, 0.f, 45.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	vSpawnPos = { 64.f, 0.f, 45.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	vSpawnPos = { 68.f, 0.f, 45.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Wolf", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;


	vSpawnPos = { 62.f, 0.f, 51.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Yeti", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	vSpawnPos = { 66.f, 0.f, 51.f };
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Yeti", SCENE_ICELAND, LayerTag, &vSpawnPos)))
		return E_FAIL;

	//GameObject_IceSlime || 1 | 41.48 | 52.52 |
	//GameObject_IceSlime || 1 | 40.6 | 58.44 |

	SLIMEINFO tSlimInfo;
	ZeroMemory(&tSlimInfo, sizeof(SLIMEINFO));
	tSlimInfo.iCurCount = 1;

	tSlimInfo.vPos = { 41.48f, 0.f, 52.52f };
	tSlimInfo.iTextureNumber = 2;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Slime", SCENE_ICELAND, LayerTag, &tSlimInfo)))
		return E_FAIL;

	tSlimInfo.iTextureNumber = 2;
	tSlimInfo.vPos = { 40.6f, 0.f, 58.44f };

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Slime", SCENE_ICELAND, LayerTag, &tSlimInfo)))
		return E_FAIL;



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

HRESULT CScene_Stage2::Setup_Layer_Projectile()
{
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

HRESULT CScene_Stage2::Setup_Layer_ActiveObject(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	EVENT_INFO tEventInfo;
	tEventInfo.iEventNo = eSceneEventID::EVNET_TRAVEL;
	tEventInfo.vSpawnPos = { 3.f, 0.f, 3.f };
	tEventInfo.iFloatOption = 1;

	if (FAILED(pManagement->Add_GameObject_InLayer(&m_pTravelTrigger, SCENE_STATIC, L"GameObject_Trigger", SCENE_ICELAND, LayerTag, &tEventInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage2::Setup_Layer_NPC(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_ICELAND, L"GameObject_Stage2NPC", SCENE_ICELAND, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage2::SetUp_Layer_IceLandQuest(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_IcelandQuest", SCENE_ICELAND, LayerTag)))
		return E_FAIL;

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

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Shop")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Inventory")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_Wand")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_PlayerSkill")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_PlayerItem")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_ICELAND, L"Layer_MainQuest")))
		return E_FAIL;

	if (FAILED(pManagement->Clear_Except(SCENE_ICELAND, SCENE_TOWN)))
	{
		PRINT_LOG(L"Failed To Clear_Except", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CScene_Stage2::Respawn_Palyer()
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
