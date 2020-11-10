#include "stdafx.h"
#include "Status.h"
#include "DamageInfo.h"
#include "IceLandQuest.h"
#include "..\Headers\Yeti.h"

USING(Client)

CYeti::CYeti(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	for (int iAll = 0; iAll < YETI_END; ++iAll)
	{
		m_pTransformCom[iAll] = nullptr;
		m_pVIBufferCom[iAll] = nullptr;
		m_pTextureCom[iAll] = nullptr;
	}
}

CYeti::CYeti(const CYeti & other)
	: CGameObject(other)
{
}

HRESULT CYeti::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CYeti::Setup_GameObject(void * pArg)
{
	if (pArg)
		m_vStartPos = *(_vec3*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	Set_Active();

	return S_OK;
}

int CYeti::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
	{
		// 처치한 몬스터 수 증가
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;
		CIceLandQuest* pIceLandQuest = (CIceLandQuest*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_IceLandQuest", 0);
		if (pIceLandQuest == nullptr)
			return E_FAIL;

		if (pIceLandQuest->Get_StartDeadCnt())
			pIceLandQuest->Dead_Monster();

		return GAMEOBJECT::DEAD;
	}

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(Attack(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(Update_State()))
		return GAMEOBJECT::WARN;

	m_pTransformCom[YETI_BASE]->Update_Transform();

	m_pTransformCom[YETI_CENTER]->Update_Transform();
	m_pTransformCom[YETI_CENTER]->Set_WorldMatrix(m_pTransformCom[YETI_CENTER]->Get_Desc().matWorld * m_pTransformCom[YETI_BASE]->Get_Desc().matWorld);
	
	if (FAILED(Setting_Part(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	m_pColliderCom->Update_Collider(m_pTransformCom[YETI_BASE]->Get_Desc().vPosition);

	return GAMEOBJECT::NOEVENT;
}

int CYeti::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(Update_HurtDelay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CYeti::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int iCnt = YETI_BODY; iCnt < YETI_END; ++iCnt)
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

HRESULT CYeti::Take_Damage(const CComponent* _pDamageComp)
{
	if (!m_bCanHurt && !_pDamageComp)
		return S_OK;

	m_pStatusCom->Set_HP(((CDamageInfo*)_pDamageComp)->Get_Desc().iMinAtt);
	
	if (m_pStatusCom->Get_Status().iHp <= 0)
	{

		m_bDead = true;
	}


	return S_OK;
}

HRESULT CYeti::Add_Component()
{
	TCHAR szName[MAX_PATH] = L"";
	TCHAR szPartName[MAX_PATH] = L"";
	_vec3 vBodyPos = {};
	CTransform::TRANSFORM_DESC tTransformDesc[YETI_END] = {};

	// For.Com_Texture
	for (int iCnt = YETI_BASE; iCnt < YETI_END; ++iCnt)
	{
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szName, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;

		//경우마다 그거에 맞게 복사해서 최종적으로 문자열 들어가게만들기
		if(iCnt == YETI_BASE)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiBody");
		else if (iCnt == YETI_CENTER)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiBody");
		else if (iCnt == YETI_BODY)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiBody");
		else if (iCnt == YETI_HEAD)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiHead");
		else if (iCnt == YETI_LEFT)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiPart");
		else if (iCnt == YETI_RIGHT)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiPart");
		else if (iCnt == YETI_LEFTLEG)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiPart");
		else if (iCnt == YETI_RIGHTLEG)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiPart");

		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_Texture%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_ICELAND, szPartName, szName, (CComponent**)&m_pTextureCom[iCnt]))) ////생성 갯수
			return E_FAIL;

		if (iCnt == YETI_BASE)
		{
			tTransformDesc[YETI_BASE].vPosition = { m_vStartPos.x , 0.f, m_vStartPos.z };
			tTransformDesc[YETI_BASE].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_BASE].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == YETI_CENTER)
		{
			tTransformDesc[YETI_CENTER].vPosition = { 0.f , 1.f, 0.f };
			tTransformDesc[YETI_CENTER].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_CENTER].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == YETI_BODY)
		{
			tTransformDesc[YETI_BODY].vPosition = { 0.f , 0.f, 0.f };
			tTransformDesc[YETI_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_BODY].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_BODY].vScale = { 1.0f , 1.0f , 1.0f };
			vBodyPos = tTransformDesc[YETI_BODY].vPosition;
		}
		else if (iCnt == YETI_HEAD)
		{
			tTransformDesc[YETI_HEAD].vPosition = { 0.f , 0.8f , -0.1f };
			tTransformDesc[YETI_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_HEAD].vScale = { 0.8f , 0.8f , 0.8f };
		}
		else if (iCnt == YETI_LEFT)
		{
			tTransformDesc[YETI_LEFT].vPosition = { 0.7f, 0.f, 0.f };
			tTransformDesc[YETI_LEFT].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_LEFT].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_LEFT].vScale = { 0.3f , 0.8f , 0.4f };
		}
		else if (iCnt == YETI_RIGHT)
		{
			tTransformDesc[YETI_RIGHT].vPosition = { -0.7f, 0.f, 0.f };
			tTransformDesc[YETI_RIGHT].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_RIGHT].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_RIGHT].vScale = { 0.3f , 0.8f , 0.4f };
		}
		else if (iCnt == YETI_LEFTLEG)
		{
			tTransformDesc[YETI_LEFTLEG].vPosition = { 0.5f, -1.f,0.f };
			tTransformDesc[YETI_LEFTLEG].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_LEFTLEG].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_LEFTLEG].vScale = { 0.3f , 1.f , 0.3f };
		}
		else if (iCnt == YETI_RIGHTLEG)
		{
			tTransformDesc[YETI_RIGHTLEG].vPosition = { -0.5f, -1.f,0.f };
			tTransformDesc[YETI_RIGHTLEG].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_RIGHTLEG].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_RIGHTLEG].vScale = { 0.3f , 1.f , 0.3f };
		}


		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_Transform%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt]))) ////생성 갯수
			return E_FAIL;

	}

	CStatus::STAT tStat;
	tStat.iCriticalRate = 20;	tStat.iCriticalChance = 10;
	tStat.iDef = 50;
	tStat.iHp = 100;			tStat.iMp = 100;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = m_pTransformCom[YETI_BASE]->Get_Desc().vPosition;
	tColDesc.fRadius = 0.5f * 2.f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	CDamageInfo::DAMAGE_DESC tDmgInfo;
	tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
	tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
	tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
	tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
	tDmgInfo.pOwner = this;

	tDmgInfo.eType = eELEMENTAL_TYPE::ICE;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;


	return S_OK;
}

HRESULT CYeti::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (FAILED(Moving(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(MoveMotion(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CYeti::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vBasePos = m_pTransformCom[YETI_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vBasePos))
	{
		m_pTransformCom[YETI_BASE]->Set_Position(vBasePos);
	}

	return S_OK;
}

HRESULT CYeti::Setting_Part(float _fDeltaTime)
{
	for (_uint iCnt = YETI_BODY; iCnt < YETI_END; ++iCnt)
	{
		m_pTransformCom[iCnt]->Update_Transform();
		m_pTransformCom[iCnt]->Set_WorldMatrix(m_pTransformCom[iCnt]->Get_Desc().matWorld * m_pTransformCom[YETI_CENTER]->Get_Desc().matWorld);
	}

	return S_OK;
}

HRESULT CYeti::Moving(float _fDeltaTime)
{
	m_fTime = _fDeltaTime;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform1");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vMonsterPos = m_pTransformCom[YETI_BASE]->Get_Desc().vPosition;
	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

	_vec3 vDir = vPlayerPos - vMonsterPos;

	float fLength = D3DXVec3Length(&vDir); // 플레이어와의 거리비교

	// 공격 거리
	if (fLength < 3.3f)
	{
		LookAtPlayer(_fDeltaTime);

		if (!m_bAttackDelay)
		{
			m_bAttack = true;
		}
		m_pTransformCom[YETI_LEFTLEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[YETI_RIGHTLEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	}
	else if(fLength < 7.f)
	{
		LookAtPlayer(_fDeltaTime);

		_vec3 vPos = m_pTransformCom[YETI_BASE]->Get_Desc().vPosition;
		_vec3 vDir = pPlayerTransform->Get_Desc().vPosition - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vPos += vDir * _fDeltaTime;
		m_eCurState = CYeti::MOVE;
		m_pTransformCom[YETI_BASE]->Set_Position(vPos);
		return S_OK;
	}

	m_eCurState = IDLE;
	return S_OK;
}

HRESULT CYeti::Attack(float _fDeltaTime)
{
	if (m_bAttack)
	{
		m_bAttackDelay = true;
		m_fAttackTime += _fDeltaTime;

		if (m_fAttackTime < 0.8f)
		{
			m_pTransformCom[YETI_RIGHT]->Turn(CTransform::AXIS_X, +_fDeltaTime * 3.f);
			m_bRHandDown = true;
		}
		else if (m_fAttackTime > 0.4f && m_bRHandDown)
		{

			if (FAILED(Spawn_Snow(L"Layer_MonsterAtk")))
				return E_FAIL;
			m_fAttackTime = 0.f;
			m_bHighestCheck = true;
			m_bAttack = false;
		}
	}
	else if (m_bHighestCheck)
	{
		m_fAttackTime += _fDeltaTime;

		if (m_fAttackTime < 0.8f)
		{

			m_pTransformCom[YETI_RIGHT]->Turn(CTransform::AXIS_X, -_fDeltaTime * 3.f);
		}
		else if (m_fAttackTime > 0.8f)
		{
			m_bAttackDelay = false;
			m_bHighestCheck = false;
			m_fAttackTime = 0.f;
		}
	}

	// 최종적으로다끝나서 돌아오면 m_bAttackDelay = false;
	return S_OK;
}

HRESULT CYeti::Update_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CYeti::IDLE:
			m_pTransformCom[YETI_LEFTLEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[YETI_RIGHTLEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			break;
		case Client::CYeti::MOVE:
			break;
		case Client::CYeti::ATTACK:
			break;
		case Client::CYeti::DEAD:
			break;
		}

		m_eCurState = m_ePreState;
	}

	return S_OK;
}

HRESULT CYeti::MoveMotion(_float _fDeltaTime)
{
	if (m_eCurState == CYeti::MOVE)
	{
		m_fMoveTime += _fDeltaTime;

		if (m_fMoveTime >= 0.5f)
		{
			if (m_eMove == CYeti::CHANGE_LEFT)
				m_eMove = CYeti::CHANGE_RIGHT;
			else if (m_eMove == CYeti::CHANGE_RIGHT)
				m_eMove = CYeti::CHANGE_LEFT;

			m_fMoveTime = 0.f;
			m_pTransformCom[YETI_LEFTLEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[YETI_RIGHTLEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));

		}

		if (m_eMove == CYeti::CHANGE_LEFT)
		{
			m_pTransformCom[YETI_LEFTLEG]->Turn(CTransform::AXIS_X, -_fDeltaTime);
			m_pTransformCom[YETI_RIGHTLEG]->Turn(CTransform::AXIS_X , _fDeltaTime);
		}
		else if (m_eMove == CYeti::CHANGE_RIGHT)
		{
			m_pTransformCom[YETI_LEFTLEG]->Turn(CTransform::AXIS_X, _fDeltaTime);
			m_pTransformCom[YETI_RIGHTLEG]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		}
	}
	else
	{
		m_pTransformCom[YETI_LEFTLEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[YETI_RIGHTLEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	}


	return S_OK;
}

CYeti * CYeti::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CYeti* pInstance = new CYeti(pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CYeti", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CYeti::Clone_GameObject(void * pArg)
{
	CYeti* pInstance = new CYeti(*this);

	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CYeti", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYeti::Free()
{
	for (int iAll = 0; iAll < YETI_END; ++iAll)
	{
		Safe_Release(m_pTransformCom[iAll]);
		Safe_Release(m_pVIBufferCom[iAll]);
		Safe_Release(m_pTextureCom[iAll]);
	}

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

HRESULT CYeti::LookAtPlayer(float _fDeltaTime)
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
	_vec3 vMonPos = m_pTransformCom[YETI_BASE]->Get_Desc().vPosition;
	_vec3 vMonLook = {};

	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&vMonLook, sizeof(_vec3), &m_pTransformCom[YETI_BASE]->Get_Desc().matWorld._31, sizeof(_vec3));

	_vec3 vMonToPlayer = vPlayerPos - vMonPos;

	D3DXVec3Normalize(&vMonLook, &vMonLook);
	D3DXVec3Normalize(&vMonToPlayer, &vMonToPlayer);

	float fDot = 0.f;
	float fRad = 0.f;

	fDot = D3DXVec3Dot(&vMonLook, &vMonToPlayer);
	fRad = (float)acos(fDot);

	_vec3 vMonRight = {};
	D3DXVec3Cross(&vMonRight, &_vec3(0.f, 1.f, 0.f), &vMonLook);

	D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	float fLimit = D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	if (fabsf(fLimit) < 0.2f)
		return S_OK;

	if (fLimit > 0)
	{
		m_pTransformCom[YETI_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	}
	else
	{
		m_pTransformCom[YETI_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
	}
	
	return S_OK;
}

HRESULT CYeti::Spawn_Snow(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	_vec3 vRightHandPos = {};
	memcpy_s(&vRightHandPos, sizeof(_vec3), &m_pTransformCom[YETI_RIGHT]->Get_Desc().matWorld._41, sizeof(_vec3));
	tImpact.vPosition = vRightHandPos;


	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Snow",
		pManagement->Get_CurrentSceneID(), LayerTag , &tImpact)))
		return E_FAIL;

	return S_OK;
}

void CYeti::Set_Active()
{
	m_bActive = true;
	m_eCurState = IDLE;
}

HRESULT CYeti::Update_HurtDelay(_float _fDeltaTime)
{
	if (!m_bCanHurt)
	{
		m_fHurtTimer += _fDeltaTime;
		if (m_fHurtTimer >= m_fHurtDelay)
		{
			m_fHurtTimer = 0.f;
			m_bCanHurt = true;
		}
	}

	return S_OK;
}
