#include "stdafx.h"
#include "..\Headers\Stage2_NPC.h"
#include "Player.h"
#include "IceLandQuest.h"


USING(Client)

CStage2_NPC::CStage2_NPC(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		m_pTextureCom[iCnt] = nullptr;
		m_pTransformCom[iCnt] = nullptr;
		m_pVIBufferCom[iCnt] = nullptr;
	}
}

CStage2_NPC::CStage2_NPC(const CStage2_NPC & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CStage2_NPC::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CStage2_NPC::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	return S_OK;
}

_int CStage2_NPC::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagemnet = CManagement::Get_Instance();
	if (nullptr == pManagemnet)
		return GAMEOBJECT::ERR;

	CPlayer* pPlayer = (CPlayer*)pManagemnet->Get_GameObject(pManagemnet->Get_CurrentSceneID(), L"Layer_Player");
	if (nullptr == pPlayer)
		return GAMEOBJECT::ERR;

	CIceLandQuest* pIceQuest = (CIceLandQuest*)pManagemnet->Get_GameObject(pManagemnet->Get_CurrentSceneID(), L"Layer_IceLandQuest");
	if (nullptr == pIceQuest)
		return GAMEOBJECT::ERR;

	CTransform* vPlayerTransform = (CTransform*)pManagemnet->Get_Component(
		pManagemnet->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");


	// 일정 거리 이하가 되어야 NPC에게 말 걸 수 있음
	if (pManagemnet->Key_Pressing('G'))
	{
		_vec3 vPlayerPos = vPlayerTransform->Get_Desc().vPosition;
		_vec3 vNpcPos = m_pTransformCom[0]->Get_Desc().vPosition;

		_vec3 vMoveDir = vNpcPos - vPlayerPos;
		_float fDist = D3DXVec3Length(&vMoveDir);

		// NPC에게 말걸기
		if (fDist <= 5.f)
			pIceQuest->Set_StartQuest(true);
		else
			pIceQuest->Set_StartQuest(false);
	}

	for (_uint i = 0; i < PART_END; i++)
		m_pTransformCom[i]->Update_Transform();

	return GAMEOBJECT::NOEVENT;
}

_int CStage2_NPC::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagemnet = CManagement::Get_Instance();
	if (nullptr == pManagemnet)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagemnet->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CStage2_NPC::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;


	for (_uint iCnt = PART_START; iCnt < PART_END; ++iCnt)
	{
		if (FAILED(m_pVIBufferCom[iCnt]->Set_Transform(&m_pTransformCom[iCnt]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[iCnt]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iCnt]->Render_VIBuffer()))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CStage2_NPC::Add_Component()
{
	for (_uint i = 0; i < PART_END; i++)
	{
		WCHAR szVIBuffer[MIN_STR] = L"";
		StringCchPrintf(szVIBuffer, _countof(szVIBuffer), L"Com_VIBuffer%d", i);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szVIBuffer, (CComponent**)&m_pVIBufferCom[i])))
			return E_FAIL;
	}

	if (FAILED(Add_Component_Transform()))
		return E_FAIL;
	if (FAILED(Add_Component_Texture()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage2_NPC::Add_Component_Transform()
{
	CTransform::TRANSFORM_DESC tTransformDesc[PART_END];
	ZeroMemory(tTransformDesc, sizeof(CTransform::TRANSFORM_DESC) * PART_END);
	_float fRPS_Rad = D3DXToRadian(90.f);

	//--------------------------------------------------
	// HEAD
	//--------------------------------------------------
	tTransformDesc[PART_HEAD].vPosition = { 10.f, 2.f, 5.f };
	tTransformDesc[PART_HEAD].vScale = { 1.f, 1.f, 1.f };
	tTransformDesc[PART_HEAD].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_HEAD].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// BODY
	//--------------------------------------------------
	tTransformDesc[PART_BODY].vPosition = { tTransformDesc[PART_HEAD].vPosition + _vec3(0.f, -1.f, 0.f) };
	tTransformDesc[PART_BODY].vScale = { 0.8f, 0.9f, 0.8f };
	tTransformDesc[PART_BODY].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_BODY].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// HAND_L
	//--------------------------------------------------
	tTransformDesc[PART_HAND_LEFT].vPosition = tTransformDesc[PART_BODY].vPosition + _vec3(-0.5f, 0.0f, 0.f);
	tTransformDesc[PART_HAND_LEFT].vScale = { 0.2f, 0.7f, 0.2f };
	tTransformDesc[PART_HAND_LEFT].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_HAND_LEFT].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// HAND_R
	//--------------------------------------------------
	tTransformDesc[PART_HAND_RIGHT].vPosition = tTransformDesc[PART_BODY].vPosition + _vec3(0.5f, 0.0f, 0.f);
	tTransformDesc[PART_HAND_RIGHT].vScale = { 0.2f, 0.7f, 0.2f };
	tTransformDesc[PART_HAND_RIGHT].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_HAND_RIGHT].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// FOOT_L
	//--------------------------------------------------
	tTransformDesc[PART_FOOT_LEFT].vPosition = tTransformDesc[PART_BODY].vPosition + _vec3(-0.2f, -0.8f, 0.f);
	tTransformDesc[PART_FOOT_LEFT].vScale = { 0.3f, 0.7f, 0.5f };
	tTransformDesc[PART_FOOT_LEFT].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_FOOT_LEFT].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// FOOT_R
	//--------------------------------------------------
	tTransformDesc[PART_FOOT_RIGHT].vPosition = tTransformDesc[PART_BODY].vPosition + _vec3(0.2f, -0.8f, 0.f);
	tTransformDesc[PART_FOOT_RIGHT].vScale = { 0.3f, 0.7f, 0.5f };
	tTransformDesc[PART_FOOT_RIGHT].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_FOOT_RIGHT].fRotatePerSecond = fRPS_Rad;


	//--------------------------------------------------
	// Clone
	//--------------------------------------------------
	for (_uint i = 0; i < PART_END; ++i)
	{
		WCHAR szTransform[MIN_STR] = L"";
		StringCchPrintf(szTransform, _countof(szTransform), L"Com_Transform%d", i);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szTransform, (CComponent**)&m_pTransformCom[i], &tTransformDesc[i])))
			return E_FAIL;

		_vec3 vRot = m_pTransformCom[i]->Get_Desc().vRotate;
		vRot.y = -3.f;
		m_pTransformCom[i]->Set_Rotation(vRot);
	}

	return S_OK;
}

HRESULT CStage2_NPC::Add_Component_Texture()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;

	WCHAR szTextureName[PART_END][MAX_PATH] =
	{
		L"Component_Texture_Stage2NPC_Head",

		L"Component_Texture_Stage2NPC_Body",

		L"Component_Texture_Stage2NPC_Hand",
		L"Component_Texture_Stage2NPC_Hand",

		L"Component_Texture_Stage2NPC_Foot",
		L"Component_Texture_Stage2NPC_Foot"
	};

	//--------------------------------------------------
	// Clone
	//--------------------------------------------------
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		WCHAR szTexture[MIN_STR] = L"";
		StringCchPrintf(szTexture, _countof(szTexture), L"Com_Texture%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureName[iCnt], szTexture, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

CStage2_NPC * CStage2_NPC::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CStage2_NPC* pInstance = new CStage2_NPC(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CStage2_NPC", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStage2_NPC::Clone_GameObject(void * _pArg)
{
	CStage2_NPC* pInstance = new CStage2_NPC(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CStage2_NPC", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage2_NPC::Free()
{
	for (_uint i = 0; i < PART_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
	}

	CGameObject::Free();
}
