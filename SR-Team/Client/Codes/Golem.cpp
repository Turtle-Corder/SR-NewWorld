#include "stdafx.h"
#include "MiniGolem.h"
#include "DamageInfo.h"
#include "MainCamera.h"
#include "Sound_Manager.h"
#include "..\Headers\Golem.h"

USING(Client)

CGolem::CGolem(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < GOLEM_END; ++iCnt)
	{
		m_pVIBufferCom[iCnt] = nullptr;
		m_pTransformCom[iCnt] = nullptr;
		m_pTextureCom[iCnt] = nullptr;
	}

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
		m_pMiniGolem[iCnt] = nullptr;
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
	_vec3	vStartingPos = {};
	if (_pArg)
		vStartingPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom[GOLEM_BASE]->Set_Position(vStartingPos);

	if (FAILED(Create_MiniGolem()))
		return E_FAIL;

	Set_Active();

	return S_OK;
}

_int CGolem::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	Update_AI();

	Update_Move(_fDeltaTime);

	IsOnTerrain();

	Update_Anim(_fDeltaTime);

	Update_State();

	Update_Transform(_fDeltaTime);

	m_pColliderCom->Update_Collider(m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition);

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{
		if (m_pMiniGolem[iCnt])
			m_pMiniGolem[iCnt]->Update_GameObject(_fDeltaTime);
	}

	return GAMEOBJECT::NOEVENT;
}

_int CGolem::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	Update_AttackDelay(_fDeltaTime);
	Update_HurtDelay(_fDeltaTime);
	Update_FlinchDelay(_fDeltaTime);

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	if (m_bFlinch)
	{
		if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
			return 0;
	}

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{
		if (m_pMiniGolem[iCnt])
			m_pMiniGolem[iCnt]->LateUpdate_GameObject(_fDeltaTime);
	}

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

	for (_int iCnt = GOLEM_BODY; iCnt < GOLEM_END; ++iCnt)
	{
		if (FAILED(m_pVIBufferCom[iCnt]->Set_Transform(&m_pTransformCom[iCnt]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[iCnt]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iCnt]->Render_VIBuffer()))
			return E_FAIL;
	}

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{
		if (m_pMiniGolem[iCnt])
			m_pMiniGolem[iCnt]->Render_NoneAlpha();
	}

	return S_OK;
}

HRESULT CGolem::Render_BlendAlpha()
{
	if (!m_bFlinch)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_int iAll = 0; iAll < GOLEM_END; ++iAll)
	{
		if (FAILED(m_pVIBufferCom[iAll]->Set_Transform(&m_pTransformCom[iAll]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pFlinchTexCom->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iAll]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

CGameObject * CGolem::Clone_GameObject(void * _pArg)
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

	Safe_Release(m_pFlinchTexCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
		Safe_Release(m_pMiniGolem[iCnt]);

	CGameObject::Free();
}

CGolem * CGolem::Create(LPDIRECT3DDEVICE9 _pDevice)
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

HRESULT CGolem::Add_Component()
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

HRESULT CGolem::Add_Component_VIBuffer()
{
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint iCnt = 0; iCnt < GOLEM_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), L"Com_VIBuffer%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szCombine, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGolem::Add_Component_Transform()
{
	_float fSpeedPerSecond = 10.f;
	_float fRotatePerSecond = D3DXToRadian(90.f);

	CTransform::TRANSFORM_DESC tTransformDesc[GOLEM_END] = {};

	tTransformDesc[GOLEM_BASE].vPosition = m_tImpact.vPosition;

	tTransformDesc[GOLEM_CENTER].vPosition = { 0.f , 2.f, 0.f };

	tTransformDesc[GOLEM_BODY].vPosition = { 0.f , 0.5f, 0.f };
	tTransformDesc[GOLEM_BODY].vScale = { 5.0f , 3.f , 3.f };

	tTransformDesc[GOLEM_HEAD].vPosition = { 0.f , 0.7f , -1.6f };
	tTransformDesc[GOLEM_HEAD].vScale = { 2.5f , 1.5f , 1.f };

	tTransformDesc[GOLEM_LEFT_ARM].vPosition = { -3.f , 0.2f , -1.f };
	tTransformDesc[GOLEM_LEFT_ARM].vScale = { 1.f , 3.f , 1.f };

	tTransformDesc[GOLEM_RIGHT_ARM].vPosition = { 3.f , 0.2f , -1.f };
	tTransformDesc[GOLEM_RIGHT_ARM].vScale = { 1.f , 3.f , 1.f };

	tTransformDesc[GOLEM_LEFT_LEG].vPosition = { -1.f , -1.5f , 0.f };
	tTransformDesc[GOLEM_LEFT_LEG].vScale = { 1.f , 2.0f , 2.f };

	tTransformDesc[GOLEM_RIGHT_LEG].vPosition = { 1.f , -1.5f , 0.f };
	tTransformDesc[GOLEM_RIGHT_LEG].vScale = { 1.f , 2.0f , 2.f };

	TCHAR szCombine[MIN_STR] = L"";

	for (_uint iCnt = 0; iCnt < GOLEM_END; ++iCnt)
	{
		tTransformDesc[iCnt].fSpeedPerSecond = fSpeedPerSecond;
		tTransformDesc[iCnt].fRotatePerSecond = fRotatePerSecond;

		StringCchPrintf(szCombine, _countof(szCombine), L"Com_Transform%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szCombine, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGolem::Add_Component_Texture()
{
	TCHAR szComponentTag[][MIN_STR] =
	{
		L"Component_Texture_BossBody",
		L"Component_Texture_BossBody",
		L"Component_Texture_BossBody",
		L"Component_Texture_BossHead",
		L"Component_Texture_BossPart",
		L"Component_Texture_BossPart",
		L"Component_Texture_BossPart",
		L"Component_Texture_BossPart"
	};

	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szCombine[MIN_STR] = L"";

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	for (_uint iCnt = 0; iCnt < GOLEM_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_VOLCANIC, szComponentTag[iCnt], szCombine, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;
	}

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Textrue_Flinch", L"Com_TexFlinch", (CComponent**)&m_pFlinchTexCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CGolem::Add_Component_Extends()
{
	CStatus::STAT tStat;
	tStat.iCriticalRate = 2;	tStat.iCriticalChance = 20;
	tStat.iDef = 100;
	tStat.iHp = 7000;
	tStat.iMinAtt = 25;			tStat.iMaxAtt = 40;
	tStat.fAttRate = 1.f;		tStat.fDefRate = 1.f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;
	tColDesc.fRadius = 0.5f * 2.f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	CDamageInfo::DAMAGE_DESC tDmgInfo;
	tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
	tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
	tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
	tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
	tDmgInfo.pOwner = this;
	tDmgInfo.eType = eELEMENTAL_TYPE::FIRE;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGolem::Update_AI()
{
	if (m_eCurState == CGolem::IDLE || m_eCurState == CGolem::MOVE)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");
		if (nullptr == pPlayerTransform)
			return E_FAIL;

		_vec3 vPlayerPosition = pPlayerTransform->Get_Desc().vPosition;
		_vec3 vMonsterPosition = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;
		m_vMoveDirection = { vPlayerPosition.x - vMonsterPosition.x , 0.f , vPlayerPosition.z - vMonsterPosition.z };


		_float	fDistance = D3DXVec3Length(&m_vMoveDirection);


		if (fDistance > m_fFollowDistance && !m_OneCheck) //10.f
		{
			m_eCurState = CGolem::IDLE;
			m_OneCheck = true;
			return S_OK;

		}
		else if (fDistance > m_fAttackDistance) //3.f
		{
			m_eCurState = CGolem::MOVE;
			return S_OK;
		}


		if (m_bCanAttack)
		{
			Phatton();
			return S_OK;
		}
	}

	return S_OK;
}

HRESULT CGolem::Update_Move(_float _fDeltaTime)
{
	if (IDLE != m_eCurState)
	{
		//--------------------------------------------------
		// 플레이어 방향으로 회전 하는 곳
		//--------------------------------------------------
		_vec3 vLook = m_pTransformCom[GOLEM_BASE]->Get_Look();/*Get_Look();*/
		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&m_vMoveDirection, &m_vMoveDirection);

		_float fDot = D3DXVec3Dot(&vLook, &m_vMoveDirection);
		_float fRad = acosf(fDot);


		_vec3 vMonLeft = {};
		D3DXVec3Cross(&vMonLeft, &_vec3(0.f, 1.f, 0.f), &vLook);

		_float fLimit = D3DXVec3Dot(&vMonLeft, &m_vMoveDirection);
		if (fabsf(fLimit) > 0.2f)
		{
			if (fLimit > 0)
				m_pTransformCom[GOLEM_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
			else
				m_pTransformCom[GOLEM_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
		}

	}

	if (m_eCurState == CGolem::MOVE)
	{
		//--------------------------------------------------
		// 이동 하는 곳
		//--------------------------------------------------
		_vec3 vAddPos = m_vMoveDirection * (_fDeltaTime * m_fMoveSpeed);
		m_pTransformCom[GOLEM_BASE]->Set_Position(m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition + vAddPos);
	}

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

	D3DXVECTOR3 vBasePos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vBasePos))
	{
		m_pTransformCom[GOLEM_BASE]->Set_Position(vBasePos);
	}

	return S_OK;
}

HRESULT CGolem::Update_Transform(_float _fDeltaTime)
{
	m_pTransformCom[GOLEM_BASE]->Update_Transform();
	m_pTransformCom[GOLEM_CENTER]->Update_Transform(m_pTransformCom[GOLEM_BASE]->Get_Desc().matWorld);

	for (_uint iCnt = GOLEM_BODY; iCnt < GOLEM_END; ++iCnt)
		m_pTransformCom[iCnt]->Update_Transform(m_pTransformCom[GOLEM_CENTER]->Get_Desc().matWorld);

	return S_OK;
}

HRESULT CGolem::Update_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CGolem::IDLE:
			Anim_Reset_Move();
			Anim_Reset_Attack();
			break;

		case CGolem::MOVE:
			break;

		case CGolem::ATTACK1:
			m_fAttackDelay = 1.5f;		// 내려찍기
			break;

		case CGolem::ATTACK2:
			m_fAttackDelay = 1.5f;		// 폭탄
			break;

		case CGolem::ATTACK3:
			m_fAttackDelay = 1.f;		// 몬스터 생성
			break;

		case CGolem::ATTACK4:
			m_fAttackDelay = 2.8f;		// 분신 소환
			break;

		case CGolem::ATTACK6:
			m_fAttackDelay = 3.1f;		// 불 소환
			break;
		}

		m_iAnimationStep = 0;
		m_fAnimationTimer = 0.f;
		m_ePreState = m_eCurState;
	}

	return S_OK;
}

HRESULT CGolem::Update_Anim(_float _fDeltaTime)
{
	switch (m_eCurState)
	{
	case Client::CGolem::IDLE:
		break;
	case Client::CGolem::MOVE:
		Update_Anim_Move(_fDeltaTime);
		break;
	case Client::CGolem::ATTACK1:
		Update_Anim_Attack1(_fDeltaTime);
		break;
	case Client::CGolem::ATTACK2:
		Update_Anim_Attack2(_fDeltaTime);
		break;
	case Client::CGolem::ATTACK3:
		Update_Anim_Attack3(_fDeltaTime);
		break;
	case Client::CGolem::ATTACK4:
		Update_Anim_Attack4(_fDeltaTime);
		break;
	case Client::CGolem::ATTACK6:
		Update_Anim_Attack6(_fDeltaTime);
		break;
	}

	return S_OK;
}

HRESULT CGolem::Anim_Reset_Move()
{
	m_pTransformCom[GOLEM_LEFT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[GOLEM_RIGHT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[GOLEM_LEFT_LEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[GOLEM_RIGHT_LEG]->Set_Rotation(_vec3(0.f, 0.f, 0.f));

	return S_OK;
}

HRESULT CGolem::Anim_Reset_Attack()
{
	m_pTransformCom[GOLEM_LEFT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom[GOLEM_RIGHT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));

	return S_OK;
}

HRESULT CGolem::Update_Anim_Move(_float _fDeltaTime)
{
	if (m_eCurState != CGolem::MOVE)
		return S_OK;

	m_fAnimationTimer += _fDeltaTime;

	if (m_fAnimationTimer >= 0.3f)
	{
		m_fAnimationTimer = 0.f;
		m_iAnimationStep = !m_iAnimationStep;
		Anim_Reset_Move();
	}

	if (m_iAnimationStep)
	{
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformCom[GOLEM_LEFT_LEG]->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_LEG]->Turn(CTransform::AXIS_X, -_fDeltaTime);
	}
	else
	{
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[GOLEM_LEFT_LEG]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_LEG]->Turn(CTransform::AXIS_X, _fDeltaTime);
	}

	return S_OK;
}

HRESULT CGolem::Update_Anim_Attack_Hand(_float _fDeltaTime)
{
	_vec3 vLHPosition = m_pTransformCom[GOLEM_LEFT_ARM]->Get_Desc().vPosition;
	_vec3 vRHPosition = m_pTransformCom[GOLEM_RIGHT_ARM]->Get_Desc().vPosition;

	if (!m_bAnimationOnce)
	{
		m_fAnimationLHPosition = vLHPosition;
		m_fAnimationRHPosition = vRHPosition;

		m_bAnimationOnce = true;
	}

	m_fAnimationPlayTime += _fDeltaTime * 0.1f;

	vLHPosition.y -= m_fAnimationPlayTime;
	vRHPosition.y -= m_fAnimationPlayTime;

	m_pTransformCom[GOLEM_LEFT_ARM]->Set_Position(_vec3(vLHPosition.x, vLHPosition.y, vLHPosition.z));
	m_pTransformCom[GOLEM_RIGHT_ARM]->Set_Position(_vec3(vRHPosition.x, vRHPosition.y, vRHPosition.z));

	return S_OK;
}

HRESULT CGolem::Update_Anim_Attack1(_float _fDeltaTime)
{
	if (m_eCurState != CGolem::ATTACK1)
		return S_OK;

	m_fAnimationTimer += _fDeltaTime;

	if (m_fAnimationTimer >= 0.6f) // 3.6f
	{
		m_fAnimationTimer = 0.f;
		++m_iAnimationStep;

		if (5 == m_iAnimationStep)
		{
			Spawn_GolemImpact(1); // 0 != 면 파티클 생성 0은 미생성
		}
		else if(m_iAnimationStep == 6)
			m_eCurState = CGolem::IDLE;

	}

	if (m_iAnimationStep <= 1)
	{
		Anim_Reset_Attack();
		m_pTransformCom[GOLEM_BODY]->Turn(CTransform::AXIS_X, -_fDeltaTime * 0.1f);
	}
	else if (m_iAnimationStep <= 3)
	{
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, _fDeltaTime * 1.5f);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, _fDeltaTime * 1.5f);
	}
	else if (m_iAnimationStep <= 4)
	{
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, -(_fDeltaTime * 3.f));
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, -(_fDeltaTime * 3.f));
		m_pTransformCom[GOLEM_BODY]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	}

	return S_OK;
}

HRESULT CGolem::Update_Anim_Attack2(_float _fDeltaTime)
{
	if (m_eCurState != CGolem::ATTACK2)
		return S_OK;

	m_fAnimationTimer += _fDeltaTime;
	if (m_fAnimationTimer >= 0.6f) // 3.0f
	{
		m_fAnimationTimer = 0.f;
		++m_iAnimationStep;

		if (m_iAnimationStep == 1)
			m_fAnimationPlayTime = 0.f;

		else if (m_iAnimationStep == 2)
		{
			m_fAnimationPlayTime = 0.f;
			Spawn_Bomb();
		}
		else if (m_iAnimationStep == 5)
			m_eCurState = CGolem::IDLE;
	}

	if (m_iAnimationStep <= 1)
	{
		Anim_Reset_Attack();
		m_pTransformCom[GOLEM_BODY]->Turn(CTransform::AXIS_X, -_fDeltaTime * 0.1f);
	}
	else if (m_iAnimationStep <= 2)
	{
		Update_Anim_Attack_Hand(_fDeltaTime);
	}
	else if (m_iAnimationStep == 4)
	{
		m_pTransformCom[GOLEM_BODY]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[GOLEM_LEFT_ARM]->Set_Position(m_fAnimationLHPosition);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Set_Position(m_fAnimationRHPosition);
	}

	return S_OK;
}

HRESULT CGolem::Update_Anim_Attack3(_float _fDeltaTime)
{
	if (m_eCurState != CGolem::ATTACK3) // 1.2f
		return S_OK;

	m_fAnimationTimer += _fDeltaTime;
	if (m_fAnimationTimer >= 0.6f)
	{
		m_fAnimationTimer = 0.f;
		++m_iAnimationStep;

		if (1 == m_iAnimationStep)
			Spawn_MonSub();

		if (m_iAnimationStep == 2)
			m_eCurState = CGolem::IDLE;
	}

	return S_OK;
}

HRESULT CGolem::Update_Anim_Attack4(_float _fDeltaTime)
{
	if (m_eCurState != CGolem::ATTACK4)
		return S_OK;

	_vec3 vRootPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

	m_fAnimationTimer += _fDeltaTime;
	if (m_fAnimationTimer >= 0.6f)
	{
			m_fAnimationTimer = 0.f;
			++m_iAnimationStep;

			if (m_iAnimationStep == 1)
				m_bMiniGolem_SetPosition = true;
			if(m_iAnimationStep == 3)
			m_eCurState = CGolem::IDLE;
	}

	if (m_iAnimationStep == 2)
	{
		if (m_bMiniGolem_SetPosition)
		{
			for (_uint iCnt = 0; iCnt < 5; ++iCnt)
			{
				_vec3 vPos = { (rand() % 40 - 20) * 0.3f, 0.f, (rand() % 20 - 20) * 0.3f };
				_vec3 vRot = { 0.f, D3DXToRadian(rand() % 360), 0.f };

				_vec3 vPosition = vRootPos + vPos;

				if (vPosition.x <= 0.f)
					vPosition.x = (_float)(rand() % 20);
				else if (vPosition.z <= 0.f)
					vPosition.z = (_float)(rand() % 20);

				m_pMiniGolem[iCnt]->Set_SpawnInfo(vPosition, vRot);
				m_pMiniGolem[iCnt]->Set_Active();
			}
			m_bMiniGolem_SetPosition = false;
		}
	}

	return S_OK;
}

HRESULT CGolem::Update_Anim_Attack6(_float _fDeltaTime)
{
	if (m_eCurState != CGolem::ATTACK6)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vRootPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

	m_fAnimationTimer += _fDeltaTime;
	if (m_fAnimationTimer >= 0.6f)
	{
		m_fAnimationTimer = 0.f;
		++m_iAnimationStep;

		if (1 == m_iAnimationStep)
		{
			m_vDirection = m_pTransformCom[GOLEM_BASE]->Get_Look();
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		}

		if (m_iAnimationStep == 5)
		{
			m_pTransformCom[GOLEM_BODY]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[GOLEM_LEFT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_pTransformCom[GOLEM_RIGHT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));

			m_eCurState = CGolem::IDLE;
		}
	}

	if (m_iAnimationStep <= 1)
	{
		m_pTransformCom[GOLEM_BODY]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_Z, _fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_Z, -_fDeltaTime);
	}
	else if (m_iAnimationStep <= 4)
	{
		m_fDashPaticle_CreateTime += _fDeltaTime;

		if (m_fDashPaticle_CreateTime >= 0.2f)
		{
			Make_DashPaticle();
			m_fDashPaticle_CreateTime = 0.f;
		}

		_vec3 vMyPosition = { m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition.x , 0.f , m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition.z };
		vMyPosition -= m_vDirection * (_fDeltaTime * 5.f);
		m_pTransformCom[GOLEM_BASE]->Set_Position(vMyPosition);
		Spawn_GolemImpact();
	}


	return S_OK;
}

//HRESULT CGolem::Update_Anim_Attack5(_float _fDeltaTime)
//{
//	if (m_eCurState != CGolem::ATTACK5)
//		return S_OK;
//
//	m_fAnimationTimer += _fDeltaTime;
//	if (m_fAnimationTimer >= 0.6f)
//	{
//		m_fAnimationTimer = 0.f;
//		++m_iAnimationStep;
//
//		if (m_iAnimationStep == 1)
//			m_fAnimationPlayTime = 0.f;
//		else if (m_iAnimationStep == 2)
//			m_fAnimationPlayTime = 0.f;
//		else if (m_iAnimationStep == 3)
//			Spawn_Fire();
//		else if (m_iAnimationStep == 5)
//			m_eCurState = CGolem::IDLE;
//	}
//
//	if (m_iAnimationStep <= 1)
//	{
//		Anim_Reset_Attack();
//		m_pTransformCom[GOLEM_BODY]->Turn(CTransform::AXIS_X, -_fDeltaTime * 0.1f);
//	}
//	else if (m_iAnimationStep <= 2)
//	{
//		Update_Anim_Attack_Hand(_fDeltaTime);
//	}
//	else if (m_iAnimationStep == 4)
//	{
//		m_pTransformCom[GOLEM_BODY]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
//		m_pTransformCom[GOLEM_LEFT_ARM]->Set_Position(m_fAnimationLHPosition);
//		m_pTransformCom[GOLEM_RIGHT_ARM]->Set_Position(m_fAnimationRHPosition);
//	}
//
//	return S_OK;
//}

void CGolem::Update_AttackDelay(_float _fDeltaTime)
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
}

void CGolem::Update_HurtDelay(_float _fDeltaTime)
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

void CGolem::Update_FlinchDelay(_float _fDeltaTime)
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
}

HRESULT CGolem::Spawn_GolemImpact(_uint iOption)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	D3DXVec3Normalize(&tImpact.vDirection, &m_pTransformCom[GOLEM_BASE]->Get_Look());
	tImpact.vPosition = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition + (tImpact.vDirection * -1.f);
	tImpact.fOption = (_float)iOption;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Golem_Impact", pManagement->Get_CurrentSceneID(), L"Layer_MonsterAtk", &tImpact)))
		return E_FAIL;

	m_bCanAttack = false;
	return S_OK;
}

HRESULT CGolem::Spawn_Bomb()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;

	for (_uint iCnt = 0; iCnt < 30; ++iCnt)
	{
		_float fX = (_float)(rand() % 10); _float fZ = (_float)(rand() % 10);

		_uint iRandX = rand() % 2;
		_uint iRandZ = rand() % 2;

		if (iRandX == 0)
			fX *= -1.f;
		if (iRandZ == 0)
			fZ *= -1.f;

		tImpact.vPosition = /*m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition*/ pPlayerTransform->Get_Desc().vPosition + _vec3(fX, 0.f, fZ);

		if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Bomb", pManagement->Get_CurrentSceneID(), L"Layer_MonsterAtk", &tImpact)))
			return E_FAIL;
	}

	m_bCanAttack = false;
	return S_OK;
}

HRESULT CGolem::Spawn_MonSub()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	tImpact.vPosition = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_MonSub", pManagement->Get_CurrentSceneID(), L"Layer_MonsterAtk")))
		return E_FAIL;

	m_bCanAttack = false;
	return S_OK;
}

HRESULT CGolem::Create_MiniGolem()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{
		m_pMiniGolem[iCnt] = CMiniGolem::Create(m_pDevice);
		if (FAILED(m_pMiniGolem[iCnt]->Setup_GameObject()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGolem::Phatton()
{
	if (m_iPattonCnt >= 5)
	{
		_uint iRand = rand() % 100;
		if (iRand < 20)	m_eCurState = ATTACK1;
		else if (iRand < 40)	m_eCurState = ATTACK2;
		else if (iRand < 60)	m_eCurState = ATTACK3;
		else if (iRand < 80)	m_eCurState = ATTACK4;
		else if (iRand < 100)	m_eCurState = ATTACK6;
	}

	if (m_iPattonCnt < 5)
	{
		switch (m_iPattonCnt)
		{
			case 0:
			{
				m_iPattonCnt += 1;
				m_eCurState = ATTACK1;
				break;
			}
			case 1:
			{
				m_iPattonCnt += 1;
				m_eCurState = ATTACK2;
				break;
			}
			case 2:
			{
				m_iPattonCnt += 1;
				m_eCurState = ATTACK3;
				break;
			}
			case 3:
			{
				m_iPattonCnt += 1;
				m_eCurState = ATTACK4;
				break;
			}
			case 4:
			{
				m_iPattonCnt += 1;
				m_eCurState = ATTACK6;
				break;
			}
		}
	}

	return S_OK;
}

//
//HRESULT CGolem::Spawn_Fire()
//{
//	CManagement* pManagement = CManagement::Get_Instance();
//	if (nullptr == pManagement)
//		return E_FAIL;
//
//	_vec3 vDir = {};
//
//	INSTANTIMPACT tImpact;
//	tImpact.pAttacker = this;
//	tImpact.pStatusComp = m_pStatusCom;
//
//	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Fire", pManagement->Get_CurrentSceneID(), L"Layer_MonsterAtk", &tImpact)))
//		return E_FAIL;
//
//	m_bCanAttack = false;
//	return S_OK;
//}

HRESULT CGolem::Take_Damage(const CComponent* _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;

	if (!m_bCanHurt)
		return S_OK;

	_float fElementalRate = 1.f;

	// 물 -> 불
	if (eELEMENTAL_TYPE::ICE == ((CDamageInfo*)_pDamageComp)->Get_Desc().eType)
		fElementalRate = 2.f;

	_int iAtk = (_int)(((CDamageInfo*)_pDamageComp)->Get_Att() * fElementalRate);
	iAtk -= m_pStatusCom->Get_Def();
	if (iAtk < 0)
		iAtk = 1;


	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_pStatusCom->Set_HP(iAtk);
	if (0 >= m_pStatusCom->Get_Status().iHp)
	{
		DROPBOX_INFO tBoxInfo;
		tBoxInfo.vPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;
		tBoxInfo.iItemNo = 2;
		tBoxInfo.bGone = false;

		if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DropItem", pManagement->Get_CurrentSceneID(), L"Layer_Active", &tBoxInfo)))
			return E_FAIL;

		pManagement->Set_SceneEvent(eSceneEventID::EVENT_CLEAR);

		CSoundManager::Get_Instance()->PlayMonster(L"Golem_dead.wav");
		m_bDead = true;
	}

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr != pCamera)
	{
		_vec3 vAddPos = {};
		memcpy_s(&vAddPos, sizeof(_vec3), &pCamera->Get_ViewMatrix()->m[0][0], sizeof(_vec3));
		FLOATING_INFO tInfo;
		tInfo.iDamage = iAtk;
		tInfo.vSpawnPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition + (vAddPos * 3.f) + _vec3(0.f, 3.f, 0.f);
		pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_DamageFloat", pManagement->Get_CurrentSceneID(), L"Layer_Effect", &tInfo);
	}

	CSoundManager::Get_Instance()->PlayEffect(L"hit.wav");
	m_bCanHurt = false;
	m_bFlinch = true;
	return S_OK;
}

HRESULT CGolem::Make_DashPaticle()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact = {};

	_float fX = 0.f;
	
	for (_int iCnt = 0; iCnt < 6; ++iCnt)
	{
		tImpact.vPosition = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition + _vec3((_float)(iCnt - 2) , 0.f , 0.f);

		if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Golem_Dash", pManagement->Get_CurrentSceneID(), L"Layer_Effect", &tImpact)))/*여기 StartPos*/
			return E_FAIL;
	}
	return S_OK;
}