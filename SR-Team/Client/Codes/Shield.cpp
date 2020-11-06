#include "stdafx.h"
#include "Player.h"
#include "..\Headers\Shield.h"

USING(Client)

CShield::CShield(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CShield::CShield(const CShield & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CShield::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CShield::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tImpact = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CShield::Update_GameObject(_float _fDeltaTime)
{
	CPlayer* pPlayer = (CPlayer*)m_tImpact.pAttacker;

	if (m_bDead)
	{
		if (pPlayer)
			pPlayer->Buff_Off(CPlayer::BUFF_SHIELD);

		return GAMEOBJECT::DEAD;
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	if (FAILED(m_pTransformCom[SHIELD_BASE]->Update_Transform()))
		return E_FAIL;
	m_pTransformCom[SHIELD_BASE]->Set_WorldMatrix(m_pTransformCom[SHIELD_BASE]->Get_Desc().matWorld * pPlayerTransform->Get_Desc().matWorld);

	if (FAILED(Movement(_fDeltaTime)))
		return E_FAIL;

	for (_uint iCnt = SHIELD_MAIN; iCnt < SHIELD_END; ++iCnt)
	{
		if (FAILED(m_pTransformCom[iCnt]->Update_Transform()))
			return E_FAIL;

		m_pTransformCom[iCnt]->Set_WorldMatrix(m_pTransformCom[iCnt]->Get_Desc().matWorld *m_pTransformCom[SHIELD_BASE]->Get_Desc().matWorld);
	}

	return GAMEOBJECT::NOEVENT;
}

_int CShield::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	Update_DeadDelay(_fDeltaTime);

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CGameObject * CShield::Clone_GameObject(void * _pArg)
{
	CShield* pInstance = new CShield(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create Clone CShield", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CShield::Render_BlendAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (m_bShieldMain_RenderCheck)
	{
		if (FAILED(m_pVIBufferCom[SHIELD_MAIN]->Set_Transform(&m_pTransformCom[SHIELD_MAIN]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[SHIELD_MAIN]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[SHIELD_MAIN]->Render_VIBuffer()))
			return E_FAIL;
	}
	else
	{
		for (_uint iCnt = SHIELD_LEFT; iCnt < SHIELD_END; ++iCnt)
		{
			if (FAILED(m_pVIBufferCom[iCnt]->Set_Transform(&m_pTransformCom[iCnt]->Get_Desc().matWorld, pCamera)))
				return E_FAIL;

			if (FAILED(m_pTextureCom[iCnt]->SetTexture(0)))
				return E_FAIL;

			if (FAILED(m_pVIBufferCom[iCnt]->Render_VIBuffer()))
				return E_FAIL;
		}
	}
	return S_OK;
}

CShield * CShield::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CShield* pInstance = new CShield(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CShield", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield::Free()
{
	for (_uint iCnt = 0; iCnt < SHIELD_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	CGameObject::Free();
}

HRESULT CShield::Add_Component()
{
	TCHAR	szName[MIN_STR] = L"";
	CTransform::TRANSFORM_DESC tTransformDesc[SHIELD_END] = {};

	for (_uint iCnt = SHIELD_BASE; iCnt < SHIELD_END; ++iCnt)
	{
		StringCchPrintf(szName, _countof(szName), L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szName, (CComponent**)&m_pVIBufferCom[iCnt])))
			return E_FAIL;

		// For.Com_Texture
		StringCchPrintf(szName, _countof(szName), L"Com_Texture%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Shield", szName, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;

		if (iCnt == SHIELD_BASE)
		{
			tTransformDesc[SHIELD_BASE].vPosition = { 0.f , 0.f , 0.f };
			tTransformDesc[SHIELD_BASE].fSpeedPerSecond = 10.f;
			tTransformDesc[SHIELD_BASE].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == SHIELD_MAIN)
		{
			tTransformDesc[SHIELD_MAIN].vPosition = { 0.f , 0.f , 0.f };
			tTransformDesc[SHIELD_MAIN].fSpeedPerSecond = 10.f;
			tTransformDesc[SHIELD_MAIN].vScale = { 2.f, 2.f, 2.f };
			tTransformDesc[SHIELD_MAIN].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == SHIELD_LEFT)
		{
			tTransformDesc[SHIELD_LEFT].vPosition = { -2.f , 0.f , 0.f };
			tTransformDesc[SHIELD_LEFT].vScale = { 2.f, 2.f, 2.f };
			tTransformDesc[SHIELD_LEFT].fSpeedPerSecond = 10.f;
			tTransformDesc[SHIELD_LEFT].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == SHIELD_RIGHT)
		{
			tTransformDesc[SHIELD_RIGHT].vPosition = { 2.f , 0.f , 0.f };
			tTransformDesc[SHIELD_RIGHT].vScale = { 2.f, 2.f, 2.f };
			tTransformDesc[SHIELD_RIGHT].fSpeedPerSecond = 10.f;
			tTransformDesc[SHIELD_RIGHT].fRotatePerSecond = D3DXToRadian(90.f);
		}

		StringCchPrintf(szName, _countof(szName), L"Com_Transform%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CShield::Movement(_float _fDeltaTime)
{
	m_fMoveTime += _fDeltaTime / 2.f;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vLook = pPlayerTransform->Get_Look();
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vLeftPos = m_pTransformCom[SHIELD_LEFT]->Get_Desc().vPosition;
	_vec3 vRightPos = m_pTransformCom[SHIELD_RIGHT]->Get_Desc().vPosition;
	_vec3 vDirection = vLeftPos - vRightPos;

	_float fLength = D3DXVec3Length(&vDirection);

	if (fLength < 0.5f)
	{
		m_bShieldMain_RenderCheck = true;
	}

	if (vLook.x > 0)
	{
		vLeftPos.x += vLook.x * m_fMoveTime;
		vRightPos.x -= vLook.x * m_fMoveTime;
	}
	else if (vLook.x < 0)
	{
		vLeftPos.x -= vLook.x * m_fMoveTime;
		vRightPos.x += vLook.x * m_fMoveTime;
	}
	m_pTransformCom[SHIELD_LEFT]->Set_Position(vLeftPos);
	m_pTransformCom[SHIELD_RIGHT]->Set_Position(vRightPos);

	return S_OK;
}

void CShield::Update_DeadDelay(_float _fDeltaTime)
{
	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime > 4.f)
		m_bDead = true;
}

