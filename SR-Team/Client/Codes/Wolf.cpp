#include "stdafx.h"
#include "..\Headers\Wolf.h"

USING(Client)

CWolf::CWolf(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CWolf::CWolf(const CWolf & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CWolf::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CWolf::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_vStartPos = *(_vec3*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;
	return S_OK;
}

_int CWolf::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_State()))
		return GAMEOBJECT::ERR;

	if (FAILED(Movement(_fDeltaTime)))
		return E_FAIL;


	if (FAILED(m_pTransformCom[WOLF_BASE]->Update_Transform()))
		return 0;

	if (FAILED(Setting_Part()))
		return E_FAIL;

	return GAMEOBJECT::NOEVENT;
}

_int CWolf::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

CGameObject * CWolf::Clone_GameObject(void * _pArg)
{
	CWolf* pInstance = new CWolf(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create CWolf Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWolf::Free()
{
	for (_uint iCnt = 0; iCnt < WOLF_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}


	CGameObject::Free();
}

CWolf * CWolf::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CWolf* pInstance = new CWolf(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CWolf", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CWolf::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc[WOLF_END] = {};
	TCHAR szComponentTag[][MIN_STR] =
	{
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Wolf_Body",
		L"Component_Texture_Wolf_Neck",
		L"Component_Texture_Wolf_Face",
		L"Component_Texture_Wolf_Mouth",
		L"Component_Texture_Wolf_Ear",
		L"Component_Texture_Wolf_Ear",
		L"Component_Texture_Wolf_Leg",
		L"Component_Texture_Wolf_Leg",
		L"Component_Texture_Wolf_Leg",
		L"Component_Texture_Wolf_Leg"
	};

	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szCombine[MIN_STR] = L"";

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	for (_uint iCnt = 0; iCnt < WOLF_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szCombine, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;

		StringCchPrintf(szCombine, _countof(szCombine), szTexture, iCnt);
		if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), szComponentTag[iCnt], szCombine, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;

		if (iCnt == WOLF_BASE)
		{
			tTransformDesc[WOLF_BASE].vPosition = { m_vStartPos.x , 0.f, m_vStartPos.z };
			tTransformDesc[WOLF_BASE].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_BASE].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == WOLF_BODY)
		{
			tTransformDesc[WOLF_BODY].vPosition = { 0.f , 0.9f, -0.6f };
			tTransformDesc[WOLF_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_BODY].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_BODY].vScale = { 0.7f , 0.9f , 1.5f };
		}
		else if (iCnt == WOLF_NECK)
		{
			tTransformDesc[WOLF_NECK].vPosition = { 0.f , 0.9f, 0.5f };
			tTransformDesc[WOLF_NECK].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_NECK].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_NECK].vScale = { 1.f , 1.f , 0.7f };
		}
		else if (iCnt == WOLF_HEAD)
		{
			tTransformDesc[WOLF_HEAD].vPosition = { 0.f , 0.9f, 1.f };
			tTransformDesc[WOLF_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_HEAD].vScale = { 0.7f , 0.7f , 0.5f };
		}
		else if (iCnt == WOLF_MOUTH)
		{
			tTransformDesc[WOLF_MOUTH].vPosition = { 0.f , 0.75f, 1.3f };
			tTransformDesc[WOLF_MOUTH].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_MOUTH].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_MOUTH].vScale = { 0.4f , 0.3f , 0.3f };
		}
		else if (iCnt == WOLF_EAR1)
		{
			tTransformDesc[WOLF_EAR1].vPosition = { 0.2f , 1.3f, 1.1f };
			tTransformDesc[WOLF_EAR1].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_EAR1].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_EAR1].vScale = { 0.2f , 0.2f , 0.2f };
		}
		else if (iCnt == WOLF_EAR2)
		{
			tTransformDesc[WOLF_EAR2].vPosition = { -0.2f , 1.3f, 1.1f };
			tTransformDesc[WOLF_EAR2].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_EAR2].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_EAR2].vScale = { 0.2f , 0.2f , 0.2f };
		}
		else if (iCnt == WOLF_LEG1)
		{
			tTransformDesc[WOLF_LEG1].vPosition = { -0.2f , -0.3f , -1.f };
			tTransformDesc[WOLF_LEG1].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_LEG1].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_LEG1].vScale = { 0.3f , 1.6f ,0.3f };
		}
		else if (iCnt == WOLF_LEG2)
		{
			tTransformDesc[WOLF_LEG2].vPosition = { 0.2f, -0.3f, -1.f };
			tTransformDesc[WOLF_LEG2].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_LEG2].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_LEG2].vScale = { 0.3f , 1.6f ,0.3f };
		}
		else if (iCnt == WOLF_LEG3)
		{
			tTransformDesc[WOLF_LEG3].vPosition = { 0.2f, -0.3f, 0.3f };
			tTransformDesc[WOLF_LEG3].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_LEG3].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_LEG3].vScale = { 0.3f , 1.5f ,0.3f };
		}
		else if (iCnt == WOLF_LEG4)
		{
			tTransformDesc[WOLF_LEG4].vPosition = { -0.2f, -0.3f, 0.3f };
			tTransformDesc[WOLF_LEG4].fSpeedPerSecond = 10.f;
			tTransformDesc[WOLF_LEG4].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[WOLF_LEG4].vScale = { 0.3f , 1.5f ,0.3f };
		}

		StringCchPrintf(szCombine, _countof(szCombine), L"Com_Transform%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szCombine, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;


	}

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc[WOLF_BASE].vPosition;
	tColDesc.fRadius = 1.5f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWolf::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_int iAll = WOLF_BODY; iAll < WOLF_END; ++iAll)
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

HRESULT CWolf::Update_State()
{
	return S_OK;
}


HRESULT CWolf::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	_vec3 vPosition = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom[WOLF_BASE]->Set_Position(vPosition);
	}

	return S_OK;
}

HRESULT CWolf::Movement(_float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (FAILED(LookAtPlayer(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Move(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(MoveMotion(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Attack(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(AttackMotion(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}


HRESULT CWolf::Move(_float _fDeltaTime)
{
	if (m_eCurState != CWolf::MOVE)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 m_vPos = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;
	m_vDir = vPlayerPos - m_vPos;
	_float m_fLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	if (3.f <= m_fLength)
	{
		m_vPos += m_vDir * _fDeltaTime;
		m_pTransformCom[WOLF_BASE]->Set_Position(m_vPos);
	}
	else
		m_eCurState = CWolf::ATTACK;

	return S_OK;
}

HRESULT CWolf::LookAtPlayer(_float _fDeltaTime)
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
	_vec3 vMonPos = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;
	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&m_vLook, sizeof(_vec3), &m_pTransformCom[WOLF_BASE]->Get_Desc().matWorld._31, sizeof(_vec3));

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
		m_pTransformCom[WOLF_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	else
		m_pTransformCom[WOLF_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);

	return S_OK;
}

HRESULT CWolf::Attack(_float _fDeltaTime)
{
	if (m_eCurState != CWolf::ATTACK)
		return S_OK;

	if (!m_bCheck)
	{
		m_vPrePos = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;
		m_bCheck = true;
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 m_vPos = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;
	m_vDir = vPlayerPos - m_vPos;
	_float m_fLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	if (1.f <= m_fLength && !m_bCrash)
	{
		m_vPos += m_vDir * (_fDeltaTime * 8.f);
		m_pTransformCom[WOLF_BASE]->Set_Position(m_vPos);
	}
	else if (m_bCrash)
	{
		if (m_fLength <= 3.f)
		{
			m_vDir = m_vPos - vPlayerPos;
			_float m_fLength = D3DXVec3Length(&m_vDir);
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			m_vPos += m_vDir * _fDeltaTime * 2.f;
			m_pTransformCom[WOLF_BASE]->Set_Position(m_vPos);
		}
		else
		{
			m_eCurState = CWolf::MOVE;
			m_bCrash = false;
			m_bCheck = false;
			return S_OK;
		}
	}
	else
	{
		m_eCurState = CWolf::MOVE;
		m_bCheck = false;
	}

	return S_OK;
}

HRESULT CWolf::Setting_Part()
{
	for (_uint iCnt = WOLF_BODY; iCnt < WOLF_END; ++iCnt)
	{
		m_pTransformCom[iCnt]->Update_Transform();
		m_pTransformCom[iCnt]->Set_WorldMatrix(m_pTransformCom[iCnt]->Get_Desc().matWorld * m_pTransformCom[WOLF_BASE]->Get_Desc().matWorld);
	}

	return S_OK;
}

HRESULT CWolf::MoveMotion(_float _fDeltaTime)
{
	if (m_eCurState == CWolf::MOVE)
	{
		m_fMoveTime += _fDeltaTime;

		if (m_fMoveTime >= 0.3f)
		{
			if (m_eMove == CWolf::CHANGE_LEFT)
				m_eMove = CWolf::CHANGE_RIGHT;
			else if (m_eMove == CWolf::CHANGE_RIGHT)
				m_eMove = CWolf::CHANGE_LEFT;

			m_fMoveTime = 0.f;
			m_pTransformCom[WOLF_LEG1]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[WOLF_LEG2]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[WOLF_LEG3]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[WOLF_LEG4]->Set_Rotation(_vec3(0.f, 0.f, 0.f));

		}

		if (m_eMove == CWolf::CHANGE_LEFT)
		{
			m_pTransformCom[WOLF_LEG1]->Turn(CTransform::AXIS_X, -_fDeltaTime);
			m_pTransformCom[WOLF_LEG2]->Turn(CTransform::AXIS_X, _fDeltaTime);
			m_pTransformCom[WOLF_LEG3]->Turn(CTransform::AXIS_X, -_fDeltaTime);
			m_pTransformCom[WOLF_LEG4]->Turn(CTransform::AXIS_X, _fDeltaTime);
		}
		else if (m_eMove == CWolf::CHANGE_RIGHT)
		{
			m_pTransformCom[WOLF_LEG1]->Turn(CTransform::AXIS_X, _fDeltaTime);
			m_pTransformCom[WOLF_LEG2]->Turn(CTransform::AXIS_X, -_fDeltaTime);
			m_pTransformCom[WOLF_LEG3]->Turn(CTransform::AXIS_X, _fDeltaTime);
			m_pTransformCom[WOLF_LEG4]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		}
	}
	else
	{
		m_pTransformCom[WOLF_LEG1]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[WOLF_LEG2]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[WOLF_LEG3]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[WOLF_LEG4]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	}

	return S_OK;
}

HRESULT CWolf::AttackMotion(_float _fDeltaTime)
{
	if (m_eCurState == CWolf::ATTACK)
	{
		m_fHeadShakeTime += _fDeltaTime;

		if (m_fHeadShakeTime >= 0.6f)
		{
			if (m_eHead == CWolf::SHAKE_LHEAD)
				m_eHead = CWolf::SHAKE_RHEAD;
			else if (m_eHead == CWolf::SHAKE_RHEAD)
				m_eHead = CWolf::SHAKE_LHEAD;

			m_fHeadShakeTime = 0.f;
			m_pTransformCom[WOLF_HEAD]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[WOLF_MOUTH]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[WOLF_EAR1]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[WOLF_EAR2]->Set_Rotation(_vec3(0.f, 0.f, 0.f));

		}
		if (m_eHead == CWolf::SHAKE_LHEAD)
		{
			m_pTransformCom[WOLF_HEAD]->Turn(CTransform::AXIS_Z, _fDeltaTime);
			m_pTransformCom[WOLF_MOUTH]->Turn(CTransform::AXIS_Z, _fDeltaTime);
			m_pTransformCom[WOLF_EAR1]->Turn(CTransform::AXIS_Z, _fDeltaTime);
			m_pTransformCom[WOLF_EAR2]->Turn(CTransform::AXIS_Z, _fDeltaTime);
		}
		else if (m_eHead == CWolf::SHAKE_RHEAD)
		{
			m_pTransformCom[WOLF_HEAD]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
			m_pTransformCom[WOLF_MOUTH]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
			m_pTransformCom[WOLF_EAR1]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
			m_pTransformCom[WOLF_EAR2]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
		}
	}
	else
	{
		m_pTransformCom[WOLF_HEAD]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[WOLF_MOUTH]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[WOLF_EAR1]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[WOLF_EAR2]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	}

	return S_OK;
}

HRESULT CWolf::Spawn_InstantImpact(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_pInstantImpact->vPosition = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;
	m_pInstantImpact->pAttacker = this;


	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Instant_Impact", pManagement->Get_CurrentSceneID(), LayerTag, m_pInstantImpact)))/*여기 StartPos*/
		return E_FAIL;

	return S_OK;
}
