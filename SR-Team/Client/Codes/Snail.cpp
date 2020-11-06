#include "stdafx.h"
#include "..\Headers\Snail.h"
#include "DamageInfo.h"

USING(Client)

CSnail::CSnail(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		m_pTransformCom[iAll] = nullptr;
		m_pVIBufferCom[iAll] = nullptr;
		m_pTextureCom[iAll] = nullptr;
	}
}

CSnail::CSnail(const CSnail& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CSnail::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSnail::Setup_GameObject(void* pArg)
{
	if (pArg)
		m_vStartPos = *(_vec3*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	Set_Active();

	return S_OK;
}

int CSnail::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	if (FAILED(Update_State()))
		return GAMEOBJECT::ERR;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;


	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		if (FAILED(m_pTransformCom[iAll]->Update_Transform()))
			return 0;
	}

	if (FAILED(Setting_Part()))
		return 0;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

int CSnail::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return GAMEOBJECT::NOEVENT;

}

HRESULT CSnail::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
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

HRESULT CSnail::Add_Component()
{
	// For.Com_VIBuffer
	TCHAR szName[MAX_PATH] = L"";
	TCHAR szPartName[MAX_PATH] = L"";
	_vec3 vHeadPos = {};
	CTransform::TRANSFORM_DESC tTransformDesc[SNAIL_END] = {};

	// For.Com_Texture
	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		StringCchPrintf(szName, _countof(szName), L"Com_VIBuffer%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szName, (CComponent**)&m_pVIBufferCom[iAll]))) //생성 갯수
			return E_FAIL;

		//경우마다 그거에 맞게 복사해서 최종적으로 문자열 들어가게만들기
		if (iAll == SNAIL_HEAD)
		{
			StringCchPrintf(szPartName, _countof(szPartName), L"Component_Texture_SnailHead");
		}
		else if (iAll == SNAIL_BODY)
		{
			StringCchPrintf(szPartName, _countof(szPartName), L"Component_Texture_SnailBody");
		}

		StringCchPrintf(szName, _countof(szName), L"Com_Texture%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szPartName, szName, (CComponent**)&m_pTextureCom[iAll]))) ////생성 갯수
			return E_FAIL;

		if (iAll == SNAIL_HEAD)
		{
			tTransformDesc[SNAIL_HEAD].vPosition = { 0.f, 0.f, 0.5f };
			tTransformDesc[SNAIL_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[SNAIL_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[SNAIL_HEAD].vScale = { 0.5f , 0.5f , 0.5f };
		}
		else if (iAll == SNAIL_BODY)
		{
			tTransformDesc[SNAIL_BODY].vPosition = { m_vStartPos.x , 0.f, m_vStartPos.z };
			tTransformDesc[SNAIL_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[SNAIL_BODY].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[SNAIL_HEAD].vScale = { 1.f , 1.f , 1.f };
		}


		StringCchPrintf(szName, _countof(szName), L"Com_Transform%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iAll], &tTransformDesc[iAll]))) ////생성 갯수
			return E_FAIL;
	}

	//----------------------------------------
	// 나중에 몬스터 데이터 불러올때 지정함
	//----------------------------------------
	CStatus::STAT tStat;
	tStat.iCriticalChance = 20;	tStat.iCriticalRate = 10;
	tStat.iDef = 50;
	tStat.iHp = 100;			tStat.iMp = 100;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc[SNAIL_BODY].vPosition;
	tColDesc.fRadius = 0.5f; /* * Scale*/

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
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

HRESULT CSnail::Update_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE::IDLE:
			break;
		case STATE::MOVE:
			break;
		case STATE::ATTACK_START:
			break;
		case STATE::ATTACK_END:
			break;
		}
		m_ePreState = m_eCurState;
	}
	return S_OK;
}

HRESULT CSnail::Movement(_float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (FAILED(LookAtPlayer(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Compare_PlayerPosition()))
		return E_FAIL;

	if (FAILED(Move(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Attack(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSnail::IsOnTerrain()
{
	//--------------------------------------------------------
	// 달팽이 Terrain위로 올려줌
	//--------------------------------------------------------
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	_vec3 vPosition = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom[SNAIL_BODY]->Set_Position(vPosition);
	}


	return S_OK;
}
HRESULT CSnail::Move(_float _fDeltaTime)
{
	if (m_eCurState != CSnail::MOVE)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 vMyPos = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;
	m_vDir = vPlayerPos - vMyPos;
	_float m_fLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	if (3.f <= m_fLength)
	{
		vMyPos += m_vDir * _fDeltaTime;
		m_pTransformCom[SNAIL_BODY]->Set_Position(vMyPos);
	}
	else
	{
		m_vPrePos = vPlayerPos;
		m_eCurState = CSnail::ATTACK_START;
	}
	return S_OK;
}

HRESULT CSnail::LookAtPlayer(_float _fDeltaTime)
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
	_vec3 vMonPos = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;
	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&m_vLook, sizeof(_vec3), &m_pTransformCom[SNAIL_BODY]->Get_Desc().matWorld._31, sizeof(_vec3));

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
		m_pTransformCom[SNAIL_BODY]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	else
		m_pTransformCom[SNAIL_BODY]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);

	return S_OK;
}

HRESULT CSnail::Compare_PlayerPosition()
{
	if (m_eCurState != CSnail::IDLE)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPosition = pPlayerTransform->Get_Desc().vPosition;
	_vec3 vMyPos = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;
	_vec3 vDirection = _vec3(vPlayerPosition.x, 0.f, vPlayerPosition.z) - _vec3(vMyPos.x, 0.f, vMyPos.z);
	_float fLength = D3DXVec3Length(&vDirection);

	if (fLength < m_fCopareLength)
		m_eCurState = CSnail::MOVE;

	return S_OK;
}

HRESULT CSnail::Attack(_float _fDeltaTime)
{
	if (m_eCurState == CSnail::ATTACK_START || m_eCurState == CSnail::ATTACK_END)
	{

		_vec3 vMyPos = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;
		_vec3 vDirection = _vec3(m_vPrePos.x, 0.f, m_vPrePos.z) - _vec3(vMyPos.x, 0.f, vMyPos.z);
		_float vLength = D3DXVec3Length(&vDirection);
		D3DXVec3Normalize(&vDirection, &vDirection);

		if (m_eCurState == CSnail::ATTACK_START)
		{
			if (vLength >= 0.2f)
			{
				vMyPos += vDirection * (_fDeltaTime * 6.f);
				m_pTransformCom[SNAIL_BODY]->Set_Position(vMyPos);
			}
			else if (vLength <= 0.2f)
			{
				m_eCurState = ATTACK_END;

			}
			//if (vLength > 2.5f)
		}
		else if (m_eCurState == ATTACK_END)
		{
			vMyPos -= vDirection * (_fDeltaTime);
			m_pTransformCom[SNAIL_BODY]->Set_Position(vMyPos);

			if (vLength > 2.5f)
			{
				m_eCurState = CSnail::MOVE;
			}
		}
	}
	else
		return S_OK;

	return S_OK;
}

HRESULT CSnail::Setting_Part()
{
	m_pTransformCom[SNAIL_HEAD]->Set_WorldMatrix(m_pTransformCom[SNAIL_HEAD]->Get_Desc().matWorld * m_pTransformCom[SNAIL_BODY]->Get_Desc().matWorld);

	return S_OK;
}

HRESULT CSnail::Spawn_InstantImpact(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	_vec3 BodyPos = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;
	tImpact.vPosition = BodyPos;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Snail_Impact", pManagement->Get_CurrentSceneID(), LayerTag, &tImpact)))
		return E_FAIL;

	return S_OK;
}

CSnail* CSnail::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSnail* pInstance = new CSnail(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSnail", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSnail::Clone_GameObject(void * pArg)
{
	CSnail* pInstance = new CSnail(*this);

	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSnail", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnail::Free()
{
	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
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

HRESULT CSnail::Take_Damage(const CComponent* _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;
	//((CDamageInfo*)_pDamageComp)->Get_Desc().pOwner;
	if (!m_bTakeCheckOnece)
	{
		m_bTakeCheckOnece = true;
		m_eCurState = CSnail::IDLE;
	}

	m_pStatusCom->Set_HP(((CDamageInfo*)_pDamageComp)->Get_Desc().iMinAtt);

	if (0 >= m_pStatusCom->Get_Status().iHp)
	{
		m_bDead = true;
	}
	return S_OK;
}

void CSnail::Set_Active()
{
	m_bActive = true;
	m_eCurState = IDLE;
}
