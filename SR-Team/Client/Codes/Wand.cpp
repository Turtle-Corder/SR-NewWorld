#include "stdafx.h"
#include "DamageInfo.h"
#include "..\Headers\Wand.h"
#include "DamageInfo.h"

USING(Client)

CWand::CWand(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CWand::CWand(const CWand & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CWand::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CWand::Setup_GameObject(void * _pArg)
{
	m_vPlayer_RightHand_Pos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;


	return S_OK;
}

_int CWand::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Setting_Handle()))
		return E_FAIL;

	if (FAILED(Setting_Head()))
		return E_FAIL;

	return GAMEOBJECT::NOEVENT;
}

_int CWand::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;


	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CWand::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_uint iCnt = WAND_HANDLE; iCnt < WAND_END; ++iCnt)
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

HRESULT CWand::Add_Component()
{
	TCHAR szVIBuffer[MIN_STR] = L"Com_VIBuffer%d";
	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szTransform[MIN_STR] = L"Com_Transform%d";
	TCHAR szPartName[MIN_STR] = L"";
	TCHAR szCombine[MIN_STR] = L"";
	_vec3 vHeadPos = {};
	CTransform::TRANSFORM_DESC tTransformDesc[WAND_END] = {};

	// For.Com_Texture
	for (_int iCnt = 0; iCnt < WAND_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szVIBuffer, iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szCombine, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;

		//경우마다 그거에 맞게 복사해서 최종적으로 문자열 들어가게만들기
		if (iCnt == WAND_BASE)
			StringCchPrintf(szPartName, _countof(szPartName), L"Component_Texture_SnailHead");
		else if (iCnt == WAND_HANDLE)
			StringCchPrintf(szPartName, _countof(szPartName), L"Component_Texture_SnailHead");
		else if (iCnt == WAND_HEAD)
			StringCchPrintf(szPartName, _countof(szPartName), L"Component_Texture_SnailBody");

		StringCchPrintf(szCombine, _countof(szCombine), szTexture, iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szPartName, szCombine, (CComponent**)&m_pTextureCom[iCnt]))) ////생성 갯수
			return E_FAIL;

		if (iCnt == WAND_BASE)
		{
			tTransformDesc[WAND_BASE].vPosition = { 0.f , -0.5f , 0.5f };
			tTransformDesc[WAND_BASE].vRotate = { D3DXToRadian(-90.f), 0.f, 0.f };
			tTransformDesc[WAND_BASE].fSpeedPerSecond = 10.f;
			tTransformDesc[WAND_BASE].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WAND_BASE].vScale = { 3.5f , 2.0f , 1.5f };
		}
		if (iCnt == WAND_HANDLE)
		{
			tTransformDesc[WAND_HANDLE].vPosition = { 0.f , -0.5f , 0.f };
			tTransformDesc[WAND_HANDLE].fSpeedPerSecond = 10.f;
			tTransformDesc[WAND_HANDLE].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WAND_HANDLE].vScale = { 0.2f , 1.f , 0.2f };
		}
		else if (iCnt == WAND_HEAD)
		{
			tTransformDesc[WAND_HEAD].vPosition = { 0.f , -1.25f , 0.f };
			tTransformDesc[WAND_HEAD].vRotate = { D3DXToRadian(45.f), 0.f, D3DXToRadian(45.f) };
			tTransformDesc[WAND_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[WAND_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WAND_HEAD].vScale = { 0.5f , 0.4f , 0.5f };
		}

		StringCchPrintf(szCombine, _countof(szCombine), szTransform, iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szCombine, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt]))) ////생성 갯수
			return E_FAIL;
	}

	//m_pTransformCom[WAND_BASE]->Turn(CTransform::AXIS_X, D3DXToRadian(75.f));
	// 이제 각도를 구해주자

	return S_OK;
}

HRESULT CWand::Movement(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerRH = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform3");
	if (nullptr == pPlayerRH)
		return E_FAIL;

	m_pTransformCom[WAND_HEAD]->Turn(CTransform::AXIS_X, _fDeltaTime * D3DXToRadian(45.f));
	m_pTransformCom[WAND_HEAD]->Turn(CTransform::AXIS_Y, _fDeltaTime * D3DXToRadian(90.f));
	m_pTransformCom[WAND_HEAD]->Turn(CTransform::AXIS_Z, _fDeltaTime * D3DXToRadian(45.f));

	m_pTransformCom[WAND_BASE]->Update_Transform(pPlayerRH->Get_Desc().matWorld);

	return S_OK;
}

HRESULT CWand::Setting_Handle()
{
	m_pTransformCom[WAND_HANDLE]->Update_Transform();

	m_pTransformCom[WAND_HANDLE]->Set_WorldMatrix(m_pTransformCom[WAND_HANDLE]->Get_Desc().matWorld * m_pTransformCom[WAND_BASE]->Get_Desc().matWorld);
	return S_OK;
}

HRESULT CWand::Setting_Head()
{
	m_pTransformCom[WAND_HEAD]->Update_Transform();

	m_pTransformCom[WAND_HEAD]->Set_WorldMatrix(m_pTransformCom[WAND_HEAD]->Get_Desc().matWorld * m_pTransformCom[WAND_BASE]->Get_Desc().matWorld);
	return S_OK;
}

CGameObject* CWand::Clone_GameObject(void * _pArg)
{
	CWand* pInstance = new CWand(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CWand", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWand::Free()
{
	for (_uint iCnt = 0; iCnt < WAND_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusComp);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CWand * CWand::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CWand* pInstance = new CWand(pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CWand", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CWand::Take_Damage(const CComponent * _pDamageComp)
{

	return S_OK;
}

HRESULT CWand::Change_State(STATE _eState)
{
	if (m_eState == _eState)
		return S_OK;

	switch (m_eState)
	{
	case Client::CWand::IDLE:
		break;
	case Client::CWand::NORMAL_ATTACK:
		break;
	case Client::CWand::SKILL_ATTACK:
		break;
	default:
		break;
	}

	return S_OK;
}

