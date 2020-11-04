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
	_vec3 vInitPosition = {};

	if (_pArg)
		vInitPosition = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom[WOLF_BASE]->Set_Position(vInitPosition);

	m_tImpact.pAttacker = this;
	m_tImpact.pStatusComp = m_pStatusCom;

	return S_OK;
}

_int CWolf::Update_GameObject(_float _fDeltaTime)
{

	Update_AI();						// 1. �� ���� ������ �Ѵ�

	Update_Move(_fDeltaTime);			// 2. ���� �̵� ��Ų��

	IsOnTerrain();						// 3. y��(����)�� ���� ��Ų��.

	Update_State(_fDeltaTime);			// 4. ���� ��ȭ�� ���� �� ���� (���� Anim�ʸ�..)

	Update_Anim(_fDeltaTime);			// 5. �ִϸ��̼� ���

	Update_Transform(_fDeltaTime);		// 6. ��ĺ�ȯ

	m_pColliderCom->Update_Collider(m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition);	// 7. �浹ü �̵�

	return GAMEOBJECT::NOEVENT;
}

_int CWolf::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(Update_AtkDelay(_fDeltaTime)))
		return GAMEOBJECT::WARN;
	
	if (FAILED(Update_HurtDelay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CWolf::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_int iCnt = WOLF_BODY; iCnt < WOLF_END; ++iCnt)
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
	if (FAILED(Add_Component_VIBuffer()))
		return E_FAIL;
	
	if (FAILED(Add_Component_Transform()))
		return E_FAIL;

	if (FAILED(Add_Component_Texture()))
		return E_FAIL;

	if (FAILED(Add_Component_Extends()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWolf::Add_Component_VIBuffer()
{
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint iCnt = 0; iCnt < WOLF_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), L"Com_VIBuffer%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szCombine, (CComponent**)&m_pVIBufferCom[iCnt]))) //���� ����
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CWolf::Add_Component_Transform()
{
	_float fSPS = 10.f;
	_float fRotPS = D3DXToRadian(90.f);

	CTransform::TRANSFORM_DESC tTransformDesc[WOLF_END];
	tTransformDesc[WOLF_BASE].vPosition = {};
	tTransformDesc[WOLF_BASE].vScale = {};

	tTransformDesc[WOLF_BASE].vPosition = { 0.f , 0.9f, -0.6f };
	tTransformDesc[WOLF_BASE].vScale = { 0.7f , 0.9f , 1.5f };

	tTransformDesc[WOLF_BASE].vPosition = { 0.f , 0.9f, 0.5f };
	tTransformDesc[WOLF_BASE].vScale = { 1.f , 1.f , 0.7f };

	tTransformDesc[WOLF_BASE].vPosition = { 0.f , 0.9f, 1.f };
	tTransformDesc[WOLF_BASE].vScale = { 0.7f , 0.7f , 0.5f };

	tTransformDesc[WOLF_BASE].vPosition = { 0.f , 0.75f, 1.3f };
	tTransformDesc[WOLF_BASE].vScale = { 0.4f , 0.3f , 0.3f };

	tTransformDesc[WOLF_BASE].vPosition = { 0.2f , 1.3f, 1.1f };
	tTransformDesc[WOLF_BASE].vScale = { 0.2f , 0.2f , 0.2f };

	tTransformDesc[WOLF_BASE].vPosition = { -0.2f , 1.3f, 1.1f };
	tTransformDesc[WOLF_BASE].vScale = { 0.2f , 0.2f , 0.2f };

	tTransformDesc[WOLF_BASE].vPosition = { -0.2f , -0.3f , -1.f };
	tTransformDesc[WOLF_BASE].vScale = { 0.3f , 1.6f ,0.3f };

	tTransformDesc[WOLF_BASE].vPosition = { 0.2f, -0.3f, -1.f };
	tTransformDesc[WOLF_BASE].vScale = { 0.3f , 1.6f ,0.3f };

	tTransformDesc[WOLF_BASE].vPosition = { 0.2f, -0.3f, 0.3f };
	tTransformDesc[WOLF_BASE].vScale = { 0.3f , 1.5f ,0.3f };

	tTransformDesc[WOLF_BASE].vPosition = { -0.2f, -0.3f, 0.3f };
	tTransformDesc[WOLF_BASE].vScale = { 0.3f , 1.5f ,0.3f };

	TCHAR szCombine[MIN_STR] = L"";
	for (_uint iCnt = 0; iCnt < WOLF_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), L"Com_Transform%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szCombine, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CWolf::Add_Component_Texture()
{
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
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, iCnt);
		if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), szComponentTag[iCnt], szCombine, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CWolf::Add_Component_Extends()
{
	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;
	tColDesc.fRadius = 1.5f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;


	CStatus::STAT tStat;
	ZeroMemory(&tStat, sizeof(CStatus::STAT));
	// TODO : Stat ����
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWolf::Update_Transform(_float _fDeltaTime)
{
	m_pTransformCom[WOLF_BASE]->Update_Transform();

	for (_uint iCnt = WOLF_UPDATEA_START; iCnt < WOLF_END; ++iCnt)
	{
		if (FAILED(m_pTransformCom[iCnt]->Update_Transform(m_pTransformCom[WOLF_BASE]->Get_Desc().matWorld)))
		{
			PRINT_LOG(L"Failed To ������", LOG::DEBUG);
			return E_FAIL;
		}
	}

	return S_OK;
}


HRESULT CWolf::Update_State(_float _fDeltaTime)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CWolf::IDLE:
			break;
		
		case Client::CWolf::MOVE:
			break;
	
		case Client::CWolf::ATTACK:
			break;

		case Client::CWolf::DEAD:
			break;
		}

		m_iAnimStep = 0;
		m_fHurtTimer = 0.f;
		m_ePreState = m_eCurState;
	}

	return S_OK;
}

HRESULT CWolf::Update_AtkDelay(_float _fDeltaTime)
{
	if (!m_bAttack)
	{
		m_fAttackTimer += _fDeltaTime;
		if (m_fAttackTimer >= m_fAttackDelay)
		{
			m_fAttackTimer = 0.f;
			m_bAttack = true;
		}
	}

	return S_OK;
}

HRESULT CWolf::Update_HurtDelay(_float _fDeltaTime)
{
	if (!m_bHurt)
	{
		m_fHurtTimer += _fDeltaTime;
		if (m_fHurtTimer >= m_fHurtDelay)
		{
			m_fHurtTimer = 0.f;
			m_bHurt = true;
		}
	}

	return S_OK;
}

HRESULT CWolf::Update_Anim(_float _fDeltaTime)
{
	switch (m_eCurState)
	{

	case Client::CWolf::MOVE:
		Update_Anim_Move(_fDeltaTime);
		break;


	case Client::CWolf::ATTACK:
	{
		if (rand() % 2)
			Update_Anim_Attack1(_fDeltaTime);
		else
			Update_Anim_Attack2(_fDeltaTime);
	}
		break;


	default:
		break;
	}

	return S_OK;
}

HRESULT CWolf::Update_Anim_Move(_float _fDeltaTime)
{
	return S_OK;
}

HRESULT CWolf::Update_Anim_Attack1(_float _fDeltaTime)
{
	return S_OK;
}

HRESULT CWolf::Update_Anim_Attack2(_float _fDeltaTime)
{
	return S_OK;
}


HRESULT CWolf::Update_AI()
{
	if (IDLE == m_eCurState || MOVE == m_eCurState)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");
		if (nullptr == pPlayerTransform)
			return E_FAIL;

		_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
		_vec3 vMonsterPos = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;
		m_vMoveDirection = { vPlayerPos.x - vMonsterPos.x, 0.f, vPlayerPos.z - vMonsterPos.z };


		//--------------------------------------------------
		// ���� ������ �Ÿ����� �ָ� ������ ������ �ִ´�.
		//--------------------------------------------------
		_float fDistance = D3DXVec3Length(&m_vMoveDirection);
		if (fDistance > m_fFollowDistance)
		{
			m_eCurState = IDLE;
			return S_OK;
		}

		//--------------------------------------------------
		// ���� ���� �ۿ� �����Ƿ� �Ѿ� ����.
		//--------------------------------------------------
		if (fDistance > m_fAttackDistance)
		{
			m_eCurState = MOVE;
			return S_OK;
		}


		//--------------------------------------------------
		// ���� ���� �ȿ� �����Ƿ� ���� �Ѵ�.
		//--------------------------------------------------
		m_eCurState = ATTACK;
		return S_OK;
	}

	return S_OK;
}

HRESULT CWolf::Update_Move(_float _fDeltaTime)
{
	if (MOVE != m_eCurState)
		return S_OK;


	//--------------------------------------------------
	// ���� ������ �Ÿ����� �����̴� �ִ� ��� �̹Ƿ� 
	//  ����(MOVE)�� �ϴ� ����(ATTACK)�� �ϴ� ������ ȸ����Ų��.
	//--------------------------------------------------
	_vec3 vLook = m_pTransformCom[WOLF_BASE]->Get_Look();
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&m_vMoveDirection, &m_vMoveDirection);

	_float fDot = D3DXVec3Dot(&vLook, &m_vMoveDirection);
	_float fRad = (_float)acos(fDot);

	_vec3 vMonLeft = {};
	D3DXVec3Cross(&vMonLeft, &vLook, &_vec3(0.f, 1.f, 0.f));


	//--------------------------------------------------
	// ȸ��
	//--------------------------------------------------
	_float fLimit = D3DXVec3Dot(&vMonLeft, &m_vMoveDirection);
	if (fabsf(fLimit) > 0.2f)
	{
		if (fLimit > 0)
			m_pTransformCom[WOLF_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
		else
			m_pTransformCom[WOLF_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
	}


	//--------------------------------------------------
	// �̵�
	//--------------------------------------------------
	_vec3 vAddPos = m_vMoveDirection * m_pTransformCom[WOLF_BASE]->Get_Desc().fSpeedPerSecond;
	m_pTransformCom[WOLF_BASE]->Set_Position(m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition + vAddPos);

	return S_OK;
}

HRESULT CWolf::IsOnTerrain()
{
	if (MOVE != m_eCurState)
		return S_OK;

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

//HRESULT CWolf::Move(_float _fDeltaTime)
//{
//	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
//	_vec3 m_vPos = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;
//	m_vDir = vPlayerPos - m_vPos;
//	_float m_fLength = D3DXVec3Length(&m_vDir);
//	D3DXVec3Normalize(&m_vDir, &m_vDir);
//
//	if (3.f <= m_fLength)
//	{
//		m_vPos += m_vDir * _fDeltaTime;
//		m_pTransformCom[WOLF_BASE]->Set_Position(m_vPos);
//	}
//	else
//		m_eCurState = CWolf::ATTACK;
//
//	return S_OK;
//}


/*
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
*/

HRESULT CWolf::Spawn_InstantImpact(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_tImpact.vPosition = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Instant_Impact", pManagement->Get_CurrentSceneID(), LayerTag, &m_tImpact)))/*���� StartPos*/
		return E_FAIL;

	return S_OK;
}
