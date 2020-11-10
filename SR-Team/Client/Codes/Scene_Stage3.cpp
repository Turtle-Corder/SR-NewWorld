#include "stdafx.h"
#include "Player.h"
#include "PreLoader.h"
#include "Scene_Stage0.h"
#include "CubeTerrain.h"
#include "Sound_Manager.h"
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

	if (FAILED(Setup_Layer_Golem(L"Layer_Golem")))
		return E_FAIL;

	if (FAILED(Setup_Layer_NPC(L"Layer_NPC")))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(Setup_Layer_Projectile()))
		return E_FAIL;

	m_pPreLoader = CPreLoader::Create(m_pDevice, SCENE_TOWN);
	if (nullptr == m_pPreLoader)
	{
		PRINT_LOG(L"Failed To PreLoader Create in CScene_Stage3", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

_int CScene_Stage3::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	if (!m_bInit)
	{
		CSoundManager::Get_Instance()->StopSound(CSoundManager::BGM);
		CSoundManager::Get_Instance()->PlayBGM(L"bgm_volcanic.mp3");

		if (FAILED(Respawn_Palyer()))
			return GAMEOBJECT::WARN;

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
		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(SCENE_VOLCANIC, L"Layer_Player");
		if (nullptr == pPlayer)
			return -1;

		if (FAILED(pPlayer->Set_ClearInfo(SCENE_VOLCANIC)))
			return -1;

		if (FAILED(Travel_NextLayers()))
		{
			PRINT_LOG(L"Failed To Travel Layers in Volcanic", LOG::CLIENT);
			return -1;
		}

		// 얼음 -> 마을 ICELAND -> TOWN 단방향
		if (FAILED(pManagement->Change_CurrentScene(SCENE_TOWN, CScene_Stage0::Create(m_pDevice))))
		{
			PRINT_LOG(L"Failed To Setup CScene_Room", LOG::CLIENT);
			return -1;
		}

		if (FAILED(pManagement->ClearScene_Component_All(SCENE_VOLCANIC)))
		{
			PRINT_LOG(L"Failed To ClearScene_Component_All in Volcanic", LOG::CLIENT);
			return -1;
		}

		return 1;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CScene_Stage3::LateUpdate_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		GAMEOBJECT::WARN;

	if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_VOLCANIC, L"Layer_MonsterAtk", L"Layer_Player", L"Com_Collider", L"Com_DmgInfo")))
		return -1;

	if (FAILED(pManagement->CollisionSphere_Detection_Layers_Both(SCENE_VOLCANIC, L"Layer_PlayerAtk", L"Layer_Golem", L"Com_Collider", L"Com_DmgInfo")))
		return -1;

	return GAMEOBJECT::NOEVENT;
}

CScene_Stage3 * CScene_Stage3::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Stage3* pInstance = new CScene_Stage3(_pDevice);
	if (FAILED(pInstance->Setup_Scene()))
	{
		PRINT_LOG(L"Failed To Create CScene_Stage3", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage3::Free()
{
	Safe_Release(m_pPreLoader);

	CScene::Free();
}

HRESULT CScene_Stage3::Setup_Layer_AllObject()
{
	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_Skybox(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_Terrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DummyTerrain", SCENE_STAGE3, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_CubeTerrain(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_Environment(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_Monster(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_Golem(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vSpawnPos = { 20.f, 0.f, 20.f };

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_VOLCANIC, L"GameObject_Golem", SCENE_VOLCANIC, LayerTag, &vSpawnPos)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_Player_Attack(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_Monster_Attack(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_Projectile()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	ZeroMemory(&tImpact, sizeof(INSTANTIMPACT));
	tImpact.vPosition = { 999.f, 999.f, 999.f };

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_EnergyBolt", SCENE_VOLCANIC, L"Layer_PlayerAtk", &tImpact)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_VOLCANIC, L"GameObject_Golem_Impact", SCENE_VOLCANIC, L"Layer_MonsterAtk", &tImpact)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Setup_Layer_NPC(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE3, L"GameObject_Stage3NPC", SCENE_STAGE3, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Travel_NextLayers()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Mouse")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Player")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Item")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_MainUI")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Shop")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Inventory")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_Wand")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_PlayerSkill")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_PlayerItem")))
		return E_FAIL;

	if (FAILED(pManagement->ClearScene_Except_RegisterTag(SCENE_VOLCANIC, L"Layer_MainQuest")))
		return E_FAIL;

	if (FAILED(pManagement->Clear_Except(SCENE_VOLCANIC, SCENE_TOWN)))
	{
		PRINT_LOG(L"Failed To Clear_Except", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CScene_Stage3::Respawn_Palyer()
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
