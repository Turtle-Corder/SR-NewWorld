#include "stdafx.h"
#include "DamageInfo.h"
#include "..\Headers\Wolf.h"
#include "Sound_Manager.h"
#include "IceLandQuest.h"
USING(Client)

CWolf::CWolf(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < WOLF_END; ++iCnt)
	{
		m_pVIBufferCom[iCnt] = nullptr;
		m_pTransformCom[iCnt] = nullptr;
		m_pTextureCom[iCnt] = nullptr;
	}
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

	Set_Active();

	return S_OK;
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

	if (FAILED(Update_FlinchDelay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	if (m_bFlinch)
	{
		if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
			return 0;
	}

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

HRESULT CWolf::Render_BlendAlpha()
{
	if (!m_bFlinch)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_uint iCnt = WOLF_BODY; iCnt < WOLF_END; ++iCnt)
	{
		if (FAILED(m_pVIBufferCom[iCnt]->Set_Transform(&m_pTransformCom[iCnt]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pFlinchTexCom->SetTexture(0)))
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

HRESULT CWolf::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;

	if (!m_bCanHurt)
		return S_OK;

	_int iAtk = ((CDamageInfo*)_pDamageComp)->Get_Att();
	m_pStatusCom->Set_HP(iAtk);
	if (0 >= m_pStatusCom->Get_Status().iHp)
	{
		CSoundManager::Get_Instance()->PlayMonster(L"wolf_Dead.wav");
		m_bDead = true;
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr != pCamera)
	{
		_vec3 vAddPos = {};
		memcpy_s(&vAddPos, sizeof(_vec3), &pCamera->Get_ViewMatrix()->m[0][0], sizeof(_vec3));
		FLOATING_INFO tInfo;
		tInfo.iDamage = iAtk;
		tInfo.vSpawnPos = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition + (vAddPos * 2.f);
		pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DamageFloat", pManagement->Get_CurrentSceneID(), L"Layer_Effect", &tInfo);
	}

	CSoundManager::Get_Instance()->PlayEffect(L"hit.wav");
	m_bCanHurt = false;
	m_bFlinch = true;
	return S_OK;
}

void CWolf::Free()
{
	for (_uint iCnt = 0; iCnt < WOLF_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	Safe_Release(m_pDmgInfoCom);
	Safe_Release(m_pFlinchTexCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);

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



//----------------------------------------------------------------------------------------------------
// Add Component
//----------------------------------------------------------------------------------------------------
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
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szCombine, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
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
	tTransformDesc[WOLF_BASE].vScale = { 1.f , 1.f , 1.f };
	tTransformDesc[WOLF_BASE].vRotate = { 0.f, D3DXToRadian(180.f), 0.f };

	tTransformDesc[WOLF_BODY].vPosition = { 0.f , 0.9f, -0.6f };
	tTransformDesc[WOLF_BODY].vScale = { 0.7f , 0.9f , 1.5f };

	tTransformDesc[WOLF_NECK].vPosition = { 0.f , 0.9f, 0.5f };
	tTransformDesc[WOLF_NECK].vScale = { 1.f , 1.f , 0.7f };

	tTransformDesc[WOLF_HEAD].vPosition = { 0.f , 0.9f, 1.f };
	tTransformDesc[WOLF_HEAD].vScale = { 0.7f , 0.7f , 0.5f };

	tTransformDesc[WOLF_MOUTH].vPosition = { 0.f , 0.75f, 1.3f };
	tTransformDesc[WOLF_MOUTH].vScale = { 0.4f , 0.3f , 0.3f };

	tTransformDesc[WOLF_EAR1].vPosition = { 0.2f , 1.3f, 1.1f };
	tTransformDesc[WOLF_EAR1].vScale = { 0.2f , 0.2f , 0.2f };

	tTransformDesc[WOLF_EAR2].vPosition = { -0.2f , 1.3f, 1.1f };
	tTransformDesc[WOLF_EAR2].vScale = { 0.2f , 0.2f , 0.2f };

	tTransformDesc[WOLF_LEG1].vPosition = { -0.2f , -0.3f , -1.f };
	tTransformDesc[WOLF_LEG1].vScale = { 0.3f , 1.6f ,0.3f };

	tTransformDesc[WOLF_LEG2].vPosition = { 0.2f, -0.3f, -1.f };
	tTransformDesc[WOLF_LEG2].vScale = { 0.3f , 1.6f ,0.3f };

	tTransformDesc[WOLF_LEG3].vPosition = { 0.2f, -0.3f, 0.3f };
	tTransformDesc[WOLF_LEG3].vScale = { 0.3f , 1.5f ,0.3f };

	tTransformDesc[WOLF_LEG4].vPosition = { -0.2f, -0.3f, 0.3f };
	tTransformDesc[WOLF_LEG4].vScale = { 0.3f , 1.5f ,0.3f };

	TCHAR szCombine[MIN_STR] = L"";
	for (_uint iCnt = 0; iCnt < WOLF_END; ++iCnt)
	{
		tTransformDesc[iCnt].fSpeedPerSecond = fSPS;
		tTransformDesc[iCnt].fRotatePerSecond = fRotPS;

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
		L"Component_Texture_Wolf_Body",
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

		if (FAILED(CGameObject::Add_Component(SCENE_ICELAND, szComponentTag[iCnt], szCombine, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;
	}

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Textrue_Flinch", L"Com_TexFlinch", (CComponent**)&m_pFlinchTexCom)))
		return E_FAIL;

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
	tStat.iCriticalRate = 0;	tStat.iCriticalChance = 30;
	tStat.iDef = 10;
	tStat.iHp = 450;
	tStat.iMinAtt = 20;			tStat.iMaxAtt = 30;
	tStat.fAttRate = 1.f;		tStat.fDefRate = 1.f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;


	CDamageInfo::DAMAGE_DESC tDmgInfo;
	tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
	tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
	tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
	tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
	tDmgInfo.pOwner = this;
	tDmgInfo.eType = eELEMENTAL_TYPE::NONE;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}



//----------------------------------------------------------------------------------------------------
// Behavior
//----------------------------------------------------------------------------------------------------
_int CWolf::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	Update_AI();						// 1. 뭘 할지 결정만 한다

	Update_Move(_fDeltaTime);			// 2. 실제 이동 시킨다

	IsOnTerrain();						// 3. y축(높이)를 보정 시킨다.

	Update_State(_fDeltaTime);			// 4. 상태 변화에 따른 값 변경 (현재 Anim쪽만..)

	Update_Anim(_fDeltaTime);			// 5. 애니메이션 재생

	Update_Transform(_fDeltaTime);		// 6. 행렬변환

	m_pColliderCom->Update_Collider(m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition);	// 7. 충돌체 이동

	return GAMEOBJECT::NOEVENT;
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

HRESULT CWolf::Update_Move(_float _fDeltaTime)
{
	if (MOVE != m_eCurState)
		return S_OK;

	//--------------------------------------------------
	// 추적 가능한 거리보다 가까이는 있는 경우 이므로 
	//  추적(MOVE)을 하던 공격(ATTACK)을 하던 방향을 회전시킨다.
	//--------------------------------------------------
	_vec3 vLook = m_pTransformCom[WOLF_BASE]->Get_Look();
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
			m_pTransformCom[WOLF_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
		else
			m_pTransformCom[WOLF_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
	}


	//--------------------------------------------------
	// 이동
	//--------------------------------------------------
	_vec3 vAddPos = m_vMoveDirection * (_fDeltaTime * 2.f);
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



//----------------------------------------------------------------------------------------------------
// State
//----------------------------------------------------------------------------------------------------
HRESULT CWolf::Update_State(_float _fDeltaTime)
{
	// 상태가 바뀌는 순간 초기화시켜줄 값들!
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CWolf::IDLE:
			Anim_Reset_Move();
			break;

		case Client::CWolf::MOVE:
			break;

		case Client::CWolf::ATTACK:
			break;

		case Client::CWolf::DEAD:
			break;
		}

		m_iAnimStep = 0;
		m_fAnimTimer = 0.f;
		m_ePreState = m_eCurState;
	}

	return S_OK;
}



//----------------------------------------------------------------------------------------------------
// Transform
//----------------------------------------------------------------------------------------------------
HRESULT CWolf::Update_Transform(_float _fDeltaTime)
{
	m_pTransformCom[WOLF_BASE]->Update_Transform();

	for (_uint iCnt = WOLF_UPDATEA_START; iCnt < WOLF_END; ++iCnt)
	{
		m_pTransformCom[iCnt]->Update_Transform();

		m_pTransformCom[iCnt]->Set_WorldMatrix(m_pTransformCom[iCnt]->Get_Desc().matWorld * m_pTransformCom[WOLF_BASE]->Get_Desc().matWorld);		//(m_pTransformCom[WOLF_BASE]->Get_Desc().matWorld));
	}

	return S_OK;
}



//----------------------------------------------------------------------------------------------------
// Delay
//----------------------------------------------------------------------------------------------------
HRESULT CWolf::Update_AtkDelay(_float _fDeltaTime)
{
	if (!m_bCanAttack)
	{
		m_fAttackTimer += _fDeltaTime;
		if (m_fAttackTimer >= m_fAttackDelay)
		{
			m_fAttackTimer = 0.f;
			m_bCanAttack = true;
		}
	}

	return S_OK;
}

HRESULT CWolf::Update_HurtDelay(_float _fDeltaTime)
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

HRESULT CWolf::Update_FlinchDelay(_float _fDeltaTime)
{
	if (m_bFlinch)
	{
		m_fFlinchTimer += _fDeltaTime;
		if (m_fFlinchTimer >= m_fFlinchDealy)
		{
			m_fFlinchTimer = 0.f;
			m_bFlinch = false;
		}
	}

	return S_OK;
}



//----------------------------------------------------------------------------------------------------
// Animation
//----------------------------------------------------------------------------------------------------
HRESULT CWolf::Update_Anim(_float _fDeltaTime)
{
	switch (m_eCurState)
	{

	case Client::CWolf::MOVE:
		Update_Anim_Move(_fDeltaTime);
		break;


	case Client::CWolf::ATTACK:
	{
		//		_int iRand = rand() % 100;

		//		if (iRand < 70)								// 7할의 확률
		Update_Anim_Attack1(_fDeltaTime);
		//		else if (iRand < 90)						// 9-7할의 확률
		//			Update_Anim_Attack2(_fDeltaTime);
		//		else
		//			Update_Anim_Attack3(_fDeltaTime);		// 10-9할의 확률
	}
	break;


	default:
		break;
	}

	return S_OK;
}

HRESULT CWolf::Anim_Reset_Move()
{
	m_pTransformCom[WOLF_LEG1]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[WOLF_LEG2]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[WOLF_LEG3]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[WOLF_LEG4]->Set_Rotation(_vec3(0.f, 0.f, 0.f));

	return S_OK;
}

HRESULT CWolf::Anim_Reset_Attack()
{
	m_pTransformCom[WOLF_HEAD]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[WOLF_MOUTH]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[WOLF_EAR1]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[WOLF_EAR2]->Set_Rotation(_vec3(0.f, 0.f, 0.f));

	return S_OK;
}

HRESULT CWolf::Update_Anim_Move(_float _fDeltaTime)
{
	if (MOVE != m_eCurState)
		return S_OK;

	//--------------------------------------------------
	// 다음 애니메이션으로 바꿔줌
	//--------------------------------------------------
	m_fAnimTimer += _fDeltaTime;
	if (m_fAnimTimer >= 0.3f)
	{
		m_fAnimTimer = 0.f;
		m_iAnimStep = !m_iAnimStep;
		Anim_Reset_Move();
	}

	//--------------------------------------------------
	// 애니메이션 동작
	//--------------------------------------------------
	if (m_iAnimStep)
	{
		m_pTransformCom[WOLF_LEG1]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[WOLF_LEG2]->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformCom[WOLF_LEG3]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[WOLF_LEG4]->Turn(CTransform::AXIS_X, _fDeltaTime);
	}

	else
	{
		m_pTransformCom[WOLF_LEG1]->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformCom[WOLF_LEG2]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[WOLF_LEG3]->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformCom[WOLF_LEG4]->Turn(CTransform::AXIS_X, -_fDeltaTime);
	}

	return S_OK;
}

HRESULT CWolf::Update_Anim_Attack1(_float _fDeltaTime)
{
	if (ATTACK != m_eCurState)
		return S_OK;


	//--------------------------------------------------
	// 다음 애니메이션으로 바꿔줌
	//--------------------------------------------------
	m_fAnimTimer += _fDeltaTime;
	m_fBloodCreateTime += _fDeltaTime;

	if (m_fBloodCreateTime >= 0.2f && 5 != m_iAnimStep)
	{
		Make_Blood();
		m_fBloodCreateTime = 0.f;
	}

	if (m_fAnimTimer >= 0.4f)
	{
		m_fAnimTimer = 0.f;
		++m_iAnimStep;
		Anim_Reset_Attack();


		if (5 == m_iAnimStep)
		{
			Spawn_Impact();
			m_eCurState = IDLE;
			return S_OK;
		}

	}


	//--------------------------------------------------
	// 애니메이션 동작
	//--------------------------------------------------
	if (0 == m_iAnimStep % 2)
	{
		m_pTransformCom[WOLF_HEAD]->Turn(CTransform::AXIS_Z, _fDeltaTime);
		m_pTransformCom[WOLF_MOUTH]->Turn(CTransform::AXIS_Z, _fDeltaTime);
		m_pTransformCom[WOLF_EAR1]->Turn(CTransform::AXIS_Z, _fDeltaTime);
		m_pTransformCom[WOLF_EAR2]->Turn(CTransform::AXIS_Z, _fDeltaTime);
	}
	else
	{
		m_pTransformCom[WOLF_HEAD]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
		m_pTransformCom[WOLF_MOUTH]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
		m_pTransformCom[WOLF_EAR1]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
		m_pTransformCom[WOLF_EAR2]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
	}



	return S_OK;
}

HRESULT CWolf::Update_Anim_Attack2(_float _fDeltaTime)
{
	if (ATTACK != m_eCurState)
		return S_OK;

	//--------------------------------------------------
	// 다음 애니메이션으로 바꿔줌
	//--------------------------------------------------
	m_fAnimTimer += _fDeltaTime;
	if (m_fAnimTimer >= 0.6f)
	{
		m_fAnimTimer = 0.f;
		++m_iAnimStep;
		Anim_Reset_Attack();
	}


	//--------------------------------------------------
	// 애니메이션 동작
	//--------------------------------------------------
	if (0 == m_iAnimStep)
	{

	}

	else if (1 == m_iAnimStep)
	{

	}

	else if (2 == m_iAnimStep)
	{
		// 마지막 행동에서 바꿔줌
		m_eCurState = IDLE;
	}

	return S_OK;
}


HRESULT CWolf::Spawn_Impact()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	D3DXVec3Normalize(&tImpact.vDirection, &m_pTransformCom[WOLF_BASE]->Get_Look());
	tImpact.vPosition = m_pTransformCom[WOLF_BASE]->Get_Desc().vPosition + (tImpact.vDirection * -1.f);

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Wolf_Impact", pManagement->Get_CurrentSceneID(), L"Layer_MonsterAtk", &tImpact)))/*여기 StartPos*/
		return E_FAIL;

	m_bCanAttack = false;

	return S_OK;
}

HRESULT CWolf::Make_Blood()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.vPosition = m_pTransformCom[WOLF_HEAD]->Get_Desc().matWorld.m[3];

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Blood", pManagement->Get_CurrentSceneID(), L"Layer_Effect", &tImpact)))/*여기 StartPos*/
		return E_FAIL;

	return S_OK;
}

void CWolf::Set_Active()
{
	m_bActive = true;
	m_eCurState = IDLE;
}

