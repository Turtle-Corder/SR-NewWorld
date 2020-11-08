#include "stdafx.h"
#include "..\Headers\Stump.h"
#include "DamageInfo.h"
USING(Client)

CStump::CStump(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < STUMP_END; ++iCnt)
	{
		m_pTransformCom[iCnt] = nullptr;
		m_pVIBufferCom[iCnt] = nullptr;
		m_pTextureCom[iCnt] = nullptr;
	}
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
		m_vSpawnPos = *(_vec3*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	Set_Active();

	return S_OK;
}

_int CStump::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	Update_AI(); 

	Update_Move(_fDeltaTime);

	IsOnTerrain();

	Attack(_fDeltaTime);

	Update_State();

	Update_Transform();

	m_pColliderCom->Update_Collider(m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition);

	return GAMEOBJECT::NOEVENT;
}

_int CStump::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	Update_AttackDelay(_fDeltaTime);
	Update_HurtDelay(_fDeltaTime);

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

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

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

void CStump::Set_Active()
{
	m_bActive = true;
	m_eCurState = IDLE;
}

HRESULT CStump::Take_Damage(const CComponent * _pDamageComp)
{
	return S_OK;
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
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Stump_Part",
		L"Component_Texture_Stump_Part"
	};

	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szCombine[MIN_STR] = L"";
	TCHAR szVIBuff[MIN_STR] = L"";
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	for (_uint iCnt = 0; iCnt < STUMP_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), L"Com_VIBuffer%d", iCnt);

		if (iCnt == STUMP_LH)
			StringCchPrintf(szVIBuff, _countof(szVIBuff), L"Component_VIBuffer_Pyramid");
		else if (iCnt == STUMP_RH)
			StringCchPrintf(szVIBuff, _countof(szVIBuff), L"Component_VIBuffer_Pyramid");
		else
			StringCchPrintf(szVIBuff, _countof(szVIBuff), L"Component_VIBuffer_CubeTexture");

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szVIBuff, szCombine, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;

		StringCchPrintf(szCombine, _countof(szCombine), szTexture, iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STAGE1, szComponentTag[iCnt], szCombine, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;

		if (iCnt == STUMP_BASE)
		{
			tTransformDesc[STUMP_BASE].vPosition = { m_vSpawnPos.x , 0.f, m_vSpawnPos.z };
			tTransformDesc[STUMP_BASE].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_BASE].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == STUMP_BODY)
		{
			tTransformDesc[STUMP_BODY].vPosition = { 0.f , 2.f, 0.f };
			tTransformDesc[STUMP_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_BODY].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_BODY].vScale = { 4.0f , 4.f , 4.0f };
		}
		else if (iCnt == STUMP_LSHD)
		{
			tTransformDesc[STUMP_LSHD].vPosition = { -2.5f , 2.8f, 0.f };
			tTransformDesc[STUMP_LSHD].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LSHD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LSHD].vScale = { 1.5f , 1.5f , 1.f };
		}
		else if (iCnt == STUMP_RSHD)
		{
			tTransformDesc[STUMP_RSHD].vPosition = { 2.5f , 2.8f, 0.f };
			tTransformDesc[STUMP_RSHD].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_RSHD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_RSHD].vScale = { 1.5f , 1.5f , 1.f };
		}
		else if (iCnt == STUMP_LH)
		{
			tTransformDesc[STUMP_LH].vPosition = { -3.2f , -0.3f, 0.f };
			tTransformDesc[STUMP_LH].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LH].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LH].vScale = { 1.5f, 4.f, 1.5f };
		}
		else if (iCnt == STUMP_RH)
		{
			tTransformDesc[STUMP_RH].vPosition = { 3.2f , -0.3f, 0.f };
			tTransformDesc[STUMP_RH].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_RH].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_RH].vScale = { 1.5f, 4.f, 1.5f };
		}

		else if (iCnt == STUMP_LEG1)
		{
			tTransformDesc[STUMP_LEG1].vPosition = { -1.4f , -0.3f , -1.4f };
			tTransformDesc[STUMP_LEG1].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LEG1].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LEG1].vScale = { 1.f , 0.7f , 1.f };
		}
		else if (iCnt == STUMP_LEG2)
		{
			tTransformDesc[STUMP_LEG2].vPosition = { 1.4f, -0.3f, -1.4f };
			tTransformDesc[STUMP_LEG2].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LEG2].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LEG2].vScale = { 1.f , 0.7f , 1.f };
		}
		else if (iCnt == STUMP_LEG3)
		{
			tTransformDesc[STUMP_LEG3].vPosition = { 1.4f, -0.3f, 1.4f };
			tTransformDesc[STUMP_LEG3].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LEG3].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LEG3].vScale = { 1.f , 0.7f , 1.f };
		}
		else if (iCnt == STUMP_LEG4)
		{
			tTransformDesc[STUMP_LEG4].vPosition = { -1.4f, -0.3f, 1.4f };
			tTransformDesc[STUMP_LEG4].fSpeedPerSecond = 10.f;
			tTransformDesc[STUMP_LEG4].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[STUMP_LEG4].vScale = { 1.f , 0.7f , 1.f };
		}

		StringCchPrintf(szCombine, _countof(szCombine), L"Com_Transform%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szCombine, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;

	}

	CStatus::STAT tStat;
	tStat.iCriticalChance = 20;	tStat.iCriticalRate = 10;
	tStat.iDef = 50;
	tStat.iHp = 100;			tStat.iMp = 100;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc[STUMP_BASE].vPosition;
	tColDesc.fRadius = 0.5f * 4.f; /* * Scale*/

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	CDamageInfo::DAMAGE_DESC tDmgInfo;
	tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
	tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
	tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
	tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
	tDmgInfo.pOwner = this;
	tDmgInfo.eType = eELEMENTAL_TYPE::NONE;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStump::Update_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE::IDLE:
			break;
		case STATE::MOVE:
			break;
		case STATE::ATTACK:
			break;
		}

		m_iAnimStep = 0;
		m_fAnimTimer = 0.f;
		m_ePreState = m_eCurState;
	}
	return S_OK;
}

HRESULT CStump::Update_AI()
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
		_vec3 vMonsterPos = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;
		m_vMoveDirection = { vPlayerPos.x - vMonsterPos.x, 0.f, vPlayerPos.z - vMonsterPos.z };


		//--------------------------------------------------
		// 추적 가능한 거리보다 멀리 있으면 가만히 있는다.
		//--------------------------------------------------
		_float fDistance = D3DXVec3Length(&m_vMoveDirection);
		if (fDistance > m_fFollowDistance)
		{
			m_eCurState = IDLE;
			return S_OK;
		}

		//--------------------------------------------------
		// 공격 범위 안에 있으므로 공격 한다.
		//--------------------------------------------------
		if (m_bCanAttack && fDistance < m_fAttackDistance)
		{
			m_eCurState = ATTACK;
			return S_OK;
		}

		if (!m_bCanAttack && fDistance < m_fFollowLimitNear)
		{
			m_eCurState = IDLE;
			return S_OK;
		}

		//--------------------------------------------------
		// 공격 범위 밖에 있으므로 쫓아 간다.
		//--------------------------------------------------
		m_eCurState = MOVE;
		return S_OK;
	}

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

HRESULT CStump::Update_Move(_float _fDeltaTime)
{
	if (MOVE != m_eCurState)
		return S_OK;

	//--------------------------------------------------
	// 추적 가능한 거리보다 가까이는 있는 경우 이므로 
	//  추적(MOVE)을 하던 공격(ATTACK)을 하던 방향을 회전시킨다.
	//--------------------------------------------------
	_vec3 vLook = m_pTransformCom[STUMP_BASE]->Get_Look();
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&m_vMoveDirection, &m_vMoveDirection);

	_float fDot = D3DXVec3Dot(&vLook, &m_vMoveDirection);
	_float fRad = (_float)acos(fDot);

	_vec3 vMonLeft = {};
	D3DXVec3Cross(&vMonLeft, &vLook, &_vec3(0.f, 1.f, 0.f));


	//--------------------------------------------------
	// 회전
	//--------------------------------------------------
	_float fLimit = D3DXVec3Dot(&vMonLeft, &m_vMoveDirection);
	if (fabsf(fLimit) > 0.2f)
	{
		if (fLimit > 0)
			m_pTransformCom[STUMP_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
		else
			m_pTransformCom[STUMP_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
	}


	//--------------------------------------------------
	// 이동
	//--------------------------------------------------
	_vec3 vAddPos = m_vMoveDirection * _fDeltaTime;
	m_pTransformCom[STUMP_BASE]->Set_Position(m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition + vAddPos);

	return S_OK;
}

HRESULT CStump::Update_Transform()
{
	m_pTransformCom[STUMP_BASE]->Update_Transform();

	for (_uint iCnt = 1; iCnt < STUMP_END; ++iCnt)
	{
		m_pTransformCom[iCnt]->Update_Transform(m_pTransformCom[STUMP_BASE]->Get_Desc().matWorld);		//(m_pTransformCom[WOLF_BASE]->Get_Desc().matWorld));
	}

	return S_OK;
}

HRESULT CStump::Attack(_float _fDeltaTime)
{
	if (m_eCurState != CStump::ATTACK)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	if (m_bCanAttack)
	{
		for (_uint iCnt = 0; iCnt < 4; ++iCnt)
		{
			if (FAILED(Spawn_Acorn(L"Layer_Effect", iCnt)))
				return E_FAIL;
		}
		m_bCanAttack = false;
		m_eCurState = CStump::IDLE;
	}
	return S_OK;
}

HRESULT CStump::Spawn_Acorn(const wstring & LayerTag, _uint _iCount)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_tImpact.pAttacker = this;
	m_tImpact.pStatusComp = m_pStatusCom;
	m_tImpact.vOption.x = (_float)(_iCount + 1);

	_vec3 BodyPos = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;

	if (_iCount == 0)
		m_tImpact.vPosition = BodyPos + _vec3(-3.f, 5.f, -3.f);
	else if (_iCount == 1)
		m_tImpact.vPosition = BodyPos + _vec3(3.f, 5.f, -3.f);
	else if (_iCount == 2)
		m_tImpact.vPosition = BodyPos + _vec3(-3.f, 5.f, 3.f);
	else if (_iCount == 3)
		m_tImpact.vPosition = BodyPos + _vec3(3.f, 5.f, 3.f);

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Acorn", pManagement->Get_CurrentSceneID(), LayerTag, &m_tImpact)))
		return E_FAIL;

	return S_OK;
}

void CStump::Update_AttackDelay(_float _fDeltaTime)
{
	if (!m_bCanAttack)
	{
		m_fAttackTimer += _fDeltaTime;
		if (m_fAttackTimer >= 6.f)
		{
			m_fAttackTimer = 0.f;
			m_bCanAttack = true;
		}
	}
}

void CStump::Update_HurtDelay(_float _fDeltaTime)
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
}
