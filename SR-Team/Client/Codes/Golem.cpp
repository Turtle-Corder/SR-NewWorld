#include "stdafx.h"
#include "..\Headers\Golem.h"

USING(Client)

CGolem::CGolem(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{

}

CGolem::CGolem(const CGolem& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CGolem::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CGolem::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_vStartPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CGolem::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	m_pTransformCom[GOLEM_BASE]->Update_Transform();

	if (FAILED(Setting_Center()))
		return GAMEOBJECT::WARN;

	if (FAILED(Setting_Part()))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CGolem::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CGolem::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int iCnt = GOLEM_CENTER; iCnt < GOLEM_END; ++iCnt)
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

CGameObject* CGolem::Clone_GameObject(void * _pArg)
{
	CGolem* pInstance = new CGolem(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone Golem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem::Free()
{
	for (_uint iCnt = GOLEM_BASE; iCnt < GOLEM_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	CGameObject::Free();
}

CGolem* CGolem::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CGolem* pInstance = new CGolem(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CGolem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CGolem::Attack(_float _fDeltaTime)
{
	m_fHandAttackTime += _fDeltaTime;

	if (m_fHandAttackTime < 1.6f)
	{
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, _fDeltaTime);
	}
	else if (m_fHandAttackTime <= 1.8f && m_fHandAttackTime >= 1.6f)
	{
		m_pTransformCom[GOLEM_BODY]->Turn(CTransform::AXIS_X, -_fDeltaTime);
	}
	else if (m_fHandAttackTime >= 1.8f && m_fHandAttackTime <= 2.1f)
	{
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, (-_fDeltaTime + -_fDeltaTime) * 2.f);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, (-_fDeltaTime + -_fDeltaTime)* 2.f);
	}
	else if (m_fHandAttackTime >= 2.1f)
	{
		m_pTransformCom[GOLEM_BODY]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[GOLEM_LEFT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[GOLEM_RIGHT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_bAttack = false;
		m_fHandAttackTime = 0.f;
	}

	return S_OK;
}



HRESULT CGolem::Add_Component()
{
	m_vMonSubRand[0] = _vec3(-3.f, 0.f, -3.f);
	m_vMonSubRand[1] = _vec3(3.f, 0.f, -3.f);
	m_vMonSubRand[2] = _vec3(3.f, 0.f, 3.f);
	m_vMonSubRand[3] = _vec3(-3.f, 0.f, 3.f);

	TCHAR szName[MAX_PATH] = L"";
	TCHAR szPartName[MAX_PATH] = L"";
	TCHAR szVIBuffer[MAX_PATH] = L"";

	// For.Com_Texture
	for (int iCnt = GOLEM_BASE; iCnt < GOLEM_END; ++iCnt)
	{
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_VIBuffer%d", iCnt);

		if (iCnt == GOLEM_BASE)
			StringCchPrintf(szVIBuffer, _countof(szName), L"Component_VIBuffer_CubeTexture");
		else if (iCnt == GOLEM_CENTER)
			StringCchPrintf(szVIBuffer, _countof(szName), L"Component_VIBuffer_CubeTexture");
		else if (iCnt == GOLEM_BODY)
			StringCchPrintf(szVIBuffer, _countof(szName), L"Component_VIBuffer_CubeTexture");
		else if (iCnt == GOLEM_HEAD)
			StringCchPrintf(szVIBuffer, _countof(szName), L"Component_VIBuffer_CubeTexture");
		else if (iCnt == GOLEM_LEFT_ARM)
			StringCchPrintf(szVIBuffer, _countof(szName), L"Component_VIBuffer_CubeTexture");
		else if (iCnt == GOLEM_RIGHT_ARM)
			StringCchPrintf(szVIBuffer, _countof(szName), L"Component_VIBuffer_CubeTexture");
		else if (iCnt == GOLEM_LEFT_LEG)
			StringCchPrintf(szVIBuffer, _countof(szName), L"Component_VIBuffer_CubeTexture");
		else if (iCnt == GOLEM_RIGHT_LEG)
			StringCchPrintf(szVIBuffer, _countof(szName), L"Component_VIBuffer_CubeTexture");

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szVIBuffer , szName, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;

		//경우마다 그거에 맞게 복사해서 최종적으로 문자열 들어가게만들기
		if (iCnt == GOLEM_BASE)
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_SemiBossBody");
		else if (iCnt == GOLEM_CENTER) 
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_SemiBossBody");
		else if (iCnt == GOLEM_BODY)
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_SemiBossBody");
		else if (iCnt == GOLEM_HEAD)
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_SemiBossHead");
		else if (iCnt == GOLEM_LEFT_ARM)
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_SemiBossPart");
		else if (iCnt == GOLEM_RIGHT_ARM)
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_SemiBossPart");
		else if (iCnt == GOLEM_LEFT_LEG)
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_SemiBossPart");
		else if (iCnt == GOLEM_RIGHT_LEG)
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_SemiBossPart");

		StringCchPrintf(szName, _countof(szName), L"Com_Texture%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szPartName, szName, (CComponent**)&m_pTextureCom[iCnt]))) ////생성 갯수
			return E_FAIL;
	}

	for (_uint iCnt = GOLEM_BASE; iCnt < GOLEM_END; ++iCnt)
	{
		CTransform::TRANSFORM_DESC tTransformDesc[GOLEM_END];

		if (iCnt == GOLEM_BASE)
		{
			tTransformDesc[GOLEM_BASE].vPosition = { m_vStartPos.x , 0.f, m_vStartPos.z };
			tTransformDesc[GOLEM_BASE].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_BASE].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == GOLEM_CENTER)
		{
			tTransformDesc[GOLEM_CENTER].vPosition = { 0.f , 1.4f, 0.f };
			tTransformDesc[GOLEM_CENTER].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_CENTER].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_HEAD].vScale = { 1.f , 1.f , 1.f };
		}
		else if (iCnt == GOLEM_BODY)
		{
			tTransformDesc[GOLEM_BODY].vPosition = { 0.f , 0.f, 0.f };
			tTransformDesc[GOLEM_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_BODY].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_BODY].vScale = { 3.0f , 1.7f , 1.f };
		}
		else if (iCnt == GOLEM_HEAD)
		{
			tTransformDesc[GOLEM_HEAD].vPosition = { 0.f , 1.8f , -0.6f };
			tTransformDesc[GOLEM_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_HEAD].vScale = { 1.f , 1.f , 0.5f };
		}
		else if (iCnt == GOLEM_LEFT_ARM)
		{
			tTransformDesc[GOLEM_LEFT_ARM].vPosition = { -2.5f , 0.5f , -0.6f };
			tTransformDesc[GOLEM_LEFT_ARM].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_LEFT_ARM].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_LEFT_ARM].vScale = { 1.f , 1.4f , 0.7f };
		}
		else if (iCnt == GOLEM_RIGHT_ARM)
		{
			tTransformDesc[GOLEM_RIGHT_ARM].vPosition = { 2.5f , 0.5f , -0.6f };
			tTransformDesc[GOLEM_RIGHT_ARM].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_RIGHT_ARM].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_RIGHT_ARM].vScale = { 1.f , 1.4f , 0.7f };
		}
		else if (iCnt == GOLEM_LEFT_LEG)
		{
			tTransformDesc[GOLEM_LEFT_LEG].vPosition = { -1.f , -2.5f , 0.f };
			tTransformDesc[GOLEM_LEFT_LEG].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_LEFT_LEG].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_LEFT_LEG].vScale = { 1.5f , 2.0f , 1.f };
		}
		else if (iCnt == GOLEM_RIGHT_LEG)
		{
			tTransformDesc[GOLEM_RIGHT_LEG].vPosition = { 1.f , -2.5f , 0.f };
			tTransformDesc[GOLEM_RIGHT_LEG].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_RIGHT_LEG].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_RIGHT_LEG].vScale = { 1.5f , 2.0f , 1.f };
		}

		StringCchPrintf(szName, _countof(szName), L"Com_Transform%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt]))) ////생성 갯수
			return E_FAIL;
	}

	m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_Z , 0.4f);
	m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_Z, -0.4f);

	m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, 0.5f);
	m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, 0.5f);
	return S_OK;
}

HRESULT CGolem::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	//--------------------------------------
	// 골렘테스트용
	//--------------------------------------
	// 우선 특정상황이 되면 
	if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
		m_bLook = true;


	LookAtPlayer(_fDeltaTime);
	//Move(_fDeltaTime);

	if (m_bAttackDelay)
	{
		if (m_bAttack)
		{
			if (m_iAttackPattern % 3 == 0)
			{
				m_fAttTime += _fDeltaTime;
				Attack(_fDeltaTime);

				if (m_fAttTime >= 3.f)
				{
					m_bAttack = false;
					m_bLook = true;
					m_fAttTime = 0.f;
				}
			}
			else if (m_iAttackPattern % 3 == 1)
			{
				Create_Bomb(_fDeltaTime);

				if (!m_bAttack)
				{
					m_fAttTime = 0.f;
					m_bLook = true;
				}
			}
			else if (m_iAttackPattern % 3 == 2)
			{
				Create_MonSub(_fDeltaTime);

				if (!m_bAttack)
				{
					m_fAttTime = 0.f;
					m_bLook = true;
				}
			}
		}
		else
		{
			m_fAttTime += m_fTime;

			if (m_fAttTime >= 0.f)
			{
				m_iAttackPattern = rand() % 3;

				if (m_iAttackPattern % 3 == 0)
				{
					m_fAttTime = 0.f;
					m_bAttack = true;
				}
				else if (m_iAttackPattern % 3 == 1)
				{
					m_bAttack = true;
				}
				else if (m_iAttackPattern % 3 == 2)
				{
					m_bAttack = true;
				}
			}
		}
	}
	//--------------------------------------

	return S_OK;
}

HRESULT CGolem::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vLeftLegPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vLeftLegPos))
	{
		m_pTransformCom[GOLEM_BASE]->Set_Position(vLeftLegPos);
	}

	return S_OK;
}

HRESULT CGolem::Setting_Part()
{
	for (_uint iCnt = GOLEM_BODY; iCnt < GOLEM_END; ++iCnt)
	{
		m_pTransformCom[iCnt]->Update_Transform();
		m_pTransformCom[iCnt]->Set_WorldMatrix(m_pTransformCom[iCnt]->Get_Desc().matWorld * m_pTransformCom[GOLEM_CENTER]->Get_Desc().matWorld);
	}

	return S_OK;
}

HRESULT CGolem::Setting_Center()
{
	m_pTransformCom[GOLEM_CENTER]->Update_Transform();
	m_pTransformCom[GOLEM_CENTER]->Set_WorldMatrix(m_pTransformCom[GOLEM_CENTER]->Get_Desc().matWorld * m_pTransformCom[GOLEM_BASE]->Get_Desc().matWorld);

	return S_OK;
}

HRESULT CGolem::Create_Bomb(_float _fDeltaTime)
{

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{
		_float fRandX = _float(rand() % 20 - 10);
		_float fRandZ = _float(rand() % 20 - 10);
		m_vBombRand[iCnt] = _vec3(fRandX, 0.f, fRandZ);
	}

	if (m_bAttack && m_iCreateCnt < 6)
	{
		m_fAttTime += _fDeltaTime;

		if (m_fAttTime > 0.4f)
		{
			_vec3 vPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

			m_vTemp = { vPos.x + m_vBombRand[m_iCreateCnt].x , vPos.y , vPos.z + m_vBombRand[m_iCreateCnt].z };

			if (FAILED(Spawn_LayerBomb(L"Layer_Bomb", m_vTemp)))
				return E_FAIL;

			m_iCreateCnt += 1;
			m_fAttTime = 0.f;
		}
	}
	else if (m_iCreateCnt >= 5)
	{
		m_bAttack = false;
		m_iCreateCnt = 0;
	}

	return S_OK;
}

HRESULT CGolem::Create_MonSub(_float _fDeltaTime)
{
	if (m_iMonSubCreateCnt >= 1)
	{
		m_iMonSubCreateCnt = 0;
		m_bAttack = false;
	}

	if (m_bAttack)
	{
		for (m_iMonSubCreateCnt = 0; m_iMonSubCreateCnt < 2; ++m_iMonSubCreateCnt)
		{
			_vec3 vPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

			_vec3 m_vTemp = { vPos.x + m_vMonSubRand[m_iMonSubCreateCnt].x , vPos.y , vPos.z + m_vMonSubRand[m_iMonSubCreateCnt].z };

			if (FAILED(Spawn_Layer_MonSub(L"Layer_MonSub", m_vTemp)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CGolem::Spawn_LayerBomb(const wstring& LayerTag, _vec3 _vPos)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Bomb", pManagement->Get_CurrentSceneID(), LayerTag, &_vPos)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGolem::Spawn_Layer_MonSub(const wstring & LayerTag, _vec3 _vPos)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_MonSub", pManagement->Get_CurrentSceneID(), LayerTag, &_vPos)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGolem::Move(_float _fDeltaTIme)
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	m_vPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;
	_vec3 m_vDir = vPlayerPos - m_vPos;
	m_fLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	if (m_bAttack)
		return S_OK;

	if (2.f < m_fLength)
	{
		m_vPos += m_vDir * _fDeltaTIme;
		m_pTransformCom[GOLEM_LEFT_LEG]->Turn(CTransform::AXIS_X, -_fDeltaTIme + -_fDeltaTIme);
		m_pTransformCom[GOLEM_RIGHT_LEG]->Turn(CTransform::AXIS_X, _fDeltaTIme + _fDeltaTIme);
		m_pTransformCom[GOLEM_BASE]->Set_Position(m_vPos);
		m_bAttackDelay = false;
	}
	else if (2.f >= m_fLength && !m_bAttack)
	{
		m_bAttackDelay = true;
		m_pTransformCom[GOLEM_LEFT_LEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[GOLEM_RIGHT_LEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	}

	return S_OK;
}

HRESULT CGolem::LookAtPlayer(_float _fDeltaTime)
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
	_vec3 vMonPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;
	_vec3 vMonLook = {};

	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&vMonLook, sizeof(_vec3), &m_pTransformCom[GOLEM_BASE]->Get_Desc().matWorld._31, sizeof(_vec3));

	_vec3 vMonToPlayer = vPlayerPos - vMonPos;

	D3DXVec3Normalize(&vMonLook, &vMonLook);
	D3DXVec3Normalize(&vMonToPlayer, &vMonToPlayer);

	float fDot = 0.f;
	m_fRad = 0.f;

	fDot = D3DXVec3Dot(&vMonLook, &vMonToPlayer);
	m_fRad = (float)acos(fDot);

	_vec3 vMonRight = {};
	D3DXVec3Cross(&vMonRight, &_vec3(0.f, 1.f, 0.f), &vMonLook);

	D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	m_fLimit = D3DXVec3Dot(&vMonRight, &vMonToPlayer);
	m_bLook = false;


	if (fabsf(m_fLimit) < 0.2f)
		return S_OK;

	if (!m_bDirCheck && m_fLimit > 0.2f)
		m_pTransformCom[GOLEM_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * m_fRad);
	else if (!m_bDirCheck)
		m_pTransformCom[GOLEM_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * m_fRad);

	return S_OK;
}

HRESULT CGolem::Update_State()
{
	return S_OK;
}
