#include "stdafx.h"
#include "..\Headers\Stump.h"

USING(Client)

CStump::CStump(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CStump::CStump(const CStump & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CStump::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CStump::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_vStartPos = *(_vec3*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CStump::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(m_pTransformCom[STUMP_BASE]->Update_Transform()))
		return 0;

	if (FAILED(Setting_Part()))
		return E_FAIL;

	return GAMEOBJECT::NOEVENT;
}

_int CStump::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

CGameObject * CStump::Clone_GameObject(void * _pArg)
{
	CStump* pInstance = new CStump(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create Stump Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CStump::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_int iAll = STUMP_BODY; iAll < STUMP_END; ++iAll)
	{
		if (FAILED(m_pVIBufferCom[iAll]->Set_Transform(&m_pTransformCom[iAll]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[iAll]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iAll]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

void CStump::Free()
{
	for (_uint iCnt = 0; iCnt < STUMP_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}


	CGameObject::Free();
}

CStump * CStump::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CStump* pInstance = new CStump(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create Stump", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CStump::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc[STUMP_END] = {};
	TCHAR szComponentTag[][MIN_STR] =
	{
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Stump_Head",
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Stump_Part"
	};

	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szCombine[MIN_STR] = L"";

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	for (_uint iCnt = 0; iCnt < STUMP_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szCombine, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;

		StringCchPrintf(szCombine, _countof(szCombine), szTexture, iCnt);
		if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), szComponentTag[iCnt], szCombine, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;

		if (iCnt == STUMP_BASE)
		{
			tTransformDesc[STUMP_BASE].vPosition = { m_vStartPos.x , 0.f, m_vStartPos.z };
			tTransformDesc[STUMP_BASE].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_BASE].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == STUMP_BODY)
		{
			tTransformDesc[STUMP_BODY].vPosition = { 0.f , 0.3f, 0.f };
			tTransformDesc[STUMP_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_BODY].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == STUMP_LH)
		{
			tTransformDesc[STUMP_LH].vPosition = { -0.6f , 0.3f, 0.f };
			tTransformDesc[STUMP_LH].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LH].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LH].vScale = { 0.3f , 0.5f , 0.3f };
		}
		else if (iCnt == STUMP_RH)
		{
			tTransformDesc[STUMP_RH].vPosition = { 0.6f , 0.3f, 0.f };
			tTransformDesc[STUMP_RH].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_RH].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_RH].vScale = { 0.3f , 0.5f , 0.3f };
		}
		else if (iCnt == STUMP_LEG1)
		{
			tTransformDesc[STUMP_LEG1].vPosition = { -0.3f , -0.3f , -0.3f };
			tTransformDesc[STUMP_LEG1].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LEG1].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LEG1].vScale = { 0.3f , 0.3f , 0.3f };
		}
		else if (iCnt == STUMP_LEG2)
		{
			tTransformDesc[STUMP_LEG2].vPosition = { 0.3f, -0.3f, -0.3f };
			tTransformDesc[STUMP_LEG2].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LEG2].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LEG2].vScale = { 0.3f , 0.3f , 0.3f };
		}
		else if (iCnt == STUMP_LEG3)
		{
			tTransformDesc[STUMP_LEG3].vPosition = { 0.3f, -0.3f, 0.3f };
			tTransformDesc[STUMP_LEG3].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LEG3].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LEG3].vScale = { 0.3f , 0.3f , 0.3f };
		}
		else if (iCnt == STUMP_LEG4)
		{
			tTransformDesc[STUMP_LEG4].vPosition = { -0.3f, -0.3f, 0.3f };
			tTransformDesc[STUMP_LEG4].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LEG4].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LEG4].vScale = { 0.3f , 0.3f , 0.3f };
		}

		StringCchPrintf(szCombine, _countof(szCombine), L"Com_Transform%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szCombine, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;

	}


	return S_OK;
}

HRESULT CStump::Update_State()
{
	return S_OK;
}

HRESULT CStump::Movement(_float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (FAILED(LookAtPlayer(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Move(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStump::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	_vec3 vPosition = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom[STUMP_BASE]->Set_Position(vPosition);
	}

	return S_OK;

}

HRESULT CStump::Move(_float _fDeltaTime)
{
	if (m_eCurState != CStump::MOVE)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 m_vPos = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;
	m_vDir = vPlayerPos - m_vPos;
	_float m_fLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	if (3.f <= m_fLength)
	{
		m_vPos += m_vDir * _fDeltaTime;
		m_pTransformCom[STUMP_BASE]->Set_Position(m_vPos);
	}
	else
		m_eCurState = CStump::ATTACK;

	return S_OK;
}

HRESULT CStump::LookAtPlayer(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;
	//--------------------------------------------------
	// 플레이어와 this => Pos
	//-------------------------------------------------- 
	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 vMonPos = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;
	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&m_vLook, sizeof(_vec3), &m_pTransformCom[STUMP_BASE]->Get_Desc().matWorld._31, sizeof(_vec3));

	_vec3 vMonToPlayer = vPlayerPos - vMonPos;

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&vMonToPlayer, &vMonToPlayer);

	_float fDot = 0.f;
	_float fRad = 0.f;

	fDot = D3DXVec3Dot(&m_vLook, &vMonToPlayer);
	fRad = (_float)acos(fDot);

	_vec3 vMonRight = {};
	D3DXVec3Cross(&vMonRight, &m_vLook, &_vec3(0.f, 1.f, 0.f));

	D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	_float fLimit = D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	if (fabsf(fLimit) < 0.2f)
		return S_OK;

	if (fLimit > 0)
		m_pTransformCom[STUMP_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	else
		m_pTransformCom[STUMP_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);

	return S_OK;
}

HRESULT CStump::Attack(_float _fDeltaTime)
{
	if (m_eCurState != CStump::ATTACK)
		return S_OK;

	if (!m_bCheck)
	{
		m_vPrePos = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;
		m_bCheck = true;
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 m_vPos = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;
	m_vDir = vPlayerPos - m_vPos;
	_float m_fLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	if (0.1f <= m_fLength && !m_bCrash)
	{
		m_vPos += m_vDir * (_fDeltaTime * 8.f);
		m_pTransformCom[STUMP_BASE]->Set_Position(m_vPos);
	}
	else if (m_bCrash)
	{
		if (m_fLength <= 3.f)
		{
			m_vDir = m_vPos - vPlayerPos;
			_float m_fLength = D3DXVec3Length(&m_vDir);
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			m_vPos += m_vDir * _fDeltaTime * 2.f;
			m_pTransformCom[STUMP_BASE]->Set_Position(m_vPos);
		}
		else
		{
			m_eCurState = CStump::MOVE;
			m_bCrash = false;
			m_bCheck = false;
			return S_OK;
		}
	}
	else
	{
		m_eCurState = CStump::MOVE;
		m_bCheck = false;
	}

	return S_OK;
}

HRESULT CStump::Setting_Part()
{
	for (_uint iCnt = STUMP_BODY; iCnt < STUMP_END; ++iCnt)
	{
		m_pTransformCom[iCnt]->Update_Transform();
		m_pTransformCom[iCnt]->Set_WorldMatrix(m_pTransformCom[iCnt]->Get_Desc().matWorld * m_pTransformCom[STUMP_BASE]->Get_Desc().matWorld);
	}

	return S_OK;
}

HRESULT CStump::Spawn_InstantImpact(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_pInstantImpact->vPosition = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;
	m_pInstantImpact->pAttacker = this;


	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Instant_Impact", pManagement->Get_CurrentSceneID(), LayerTag, m_pInstantImpact)))/*여기 StartPos*/
		return E_FAIL;

	return S_OK;
}
