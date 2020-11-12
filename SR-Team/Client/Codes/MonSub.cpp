#include "stdafx.h"
#include "..\Headers\MonSub.h"
#include "DamageInfo.h"

USING(Client)

CMonSub::CMonSub(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < MONSUB_END; ++iCnt)
	{
		m_pVIBufferCom[iCnt] = nullptr;
		m_pTransformCom[iCnt] = nullptr;
		m_pTextureCom[iCnt] = nullptr;
	}

}

CMonSub::CMonSub(const CMonSub& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CMonSub::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMonSub::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CMonSub::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (Movement(_fDeltaTime))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom[MONSUB_BASE]->Update_Transform()))
		return GAMEOBJECT::WARN;


	if (FAILED(m_pTransformCom[MONSUB_RIGHT]->Update_Transform()))
		return GAMEOBJECT::WARN;

	m_pTransformCom[MONSUB_RIGHT]->Set_WorldMatrix(m_pTransformCom[MONSUB_RIGHT]->Get_Desc().matWorld * m_pTransformCom[MONSUB_BASE]->Get_Desc().matWorld);

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom[MONSUB_BASE]->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CMonSub::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return 0;
}

HRESULT CMonSub::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[MONSUB_RIGHT]->Set_Transform(&m_pTransformCom[MONSUB_RIGHT]->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[MONSUB_RIGHT]->SetTexture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[MONSUB_RIGHT]->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonSub::Take_Damage(const CComponent * _pDamageComp)
{
	m_bDead = true;

	return S_OK;
}

HRESULT CMonSub::Add_Component()
{
	TCHAR szName[MAX_PATH] = L"";
	TCHAR szPartName[MAX_PATH] = L"";
	CTransform::TRANSFORM_DESC tTransformDesc[MONSUB_END];

	for (_uint iCnt = 0; iCnt < MONSUB_END; ++iCnt)
	{

		if (iCnt == MONSUB_BASE)
		{
			tTransformDesc[iCnt].vPosition = { m_tInstant.vPosition.x , 0.f , m_tInstant.vPosition.z };
			tTransformDesc[iCnt].fSpeedPerSecond = 10.f;
			tTransformDesc[iCnt].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[iCnt].vScale = { 1.f , 1.f , 1.f };
		}
		else if (MONSUB_RIGHT)
		{
			tTransformDesc[iCnt].vPosition = { 0.f , 0.f , 0.f };
			tTransformDesc[iCnt].fSpeedPerSecond = 10.f;
			tTransformDesc[iCnt].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[iCnt].vScale = { 1.f , 1.f , 1.f };
		}
		//--------------------------------------------------
		// VIBuffer Component
		//--------------------------------------------------
		StringCchPrintf(szName, _countof(szName), L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szName, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;

		//--------------------------------------------------
		// Transform Component
		//--------------------------------------------------
		StringCchPrintf(szName, _countof(szName), L"Com_Transform%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt]))) ////생성 갯수
			return E_FAIL;
		//--------------------------------------------------
		// Texture Component
		//--------------------------------------------------
		if (iCnt == MONSUB_BASE)
		{
			StringCchPrintf(szPartName, _countof(szName), L"Component_Texture_MonSub");
		}
		else if (iCnt == MONSUB_RIGHT)
		{
			StringCchPrintf(szPartName, _countof(szPartName), L"Component_Texture_MonSub");
		}

		StringCchPrintf(szName, _countof(szName), L"Com_Texture%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_VOLCANIC, szPartName, szName, (CComponent**)&m_pTextureCom[iCnt]))) ////생성 갯수
			return E_FAIL;
	}

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = m_pTransformCom[MONSUB_BASE]->Get_Desc().vPosition;
	tColDesc.fRadius = 0.5f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;


	CStatus::STAT tStat;
	tStat.iCriticalChance = 0;	tStat.iCriticalRate = 0;
	tStat.iMinAtt = 5;			tStat.iMaxAtt = 20;
	tStat.fAttRate = 1.f;		tStat.fDefRate = 1.f;


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CStatus* pOwnerStatusComp = (CStatus*)m_tInstant.pStatusComp;
	CDamageInfo::DAMAGE_DESC tDmgInfo;
	if (pOwnerStatusComp)
	{
		tDmgInfo.pOwner = m_tInstant.pAttacker;

		// ex) yeti의 공격력 + 눈덩이 자체의 공격력 -> player의 기본 공격력 + 스태프의 공격력
		tDmgInfo.iMinAtt = pOwnerStatusComp->Get_Status().iMinAtt + m_pStatusCom->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = pOwnerStatusComp->Get_Status().iMaxAtt + m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalChance = pOwnerStatusComp->Get_Status().iCriticalChance + m_pStatusCom->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = pOwnerStatusComp->Get_Status().iCriticalRate + m_pStatusCom->Get_Status().iCriticalRate;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
		tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
	}

	tDmgInfo.eType = NONE;


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;




	return S_OK;
}

HRESULT CMonSub::Movement(_float _fDeltaTime)
{
	if (!m_bIsClone)
	{
		if (FAILED(Setting_Dir()))
			return E_FAIL;
	}

	if (FAILED(IsOnTerrain(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Move(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(LookAtPlayer(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Roll(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonSub::IsOnTerrain(_float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom[MONSUB_BASE]->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPosition = m_pTransformCom[MONSUB_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom[MONSUB_BASE]->Set_Position(vPosition);
		m_bMoveOn = true;
	}



	return S_OK;
}
HRESULT CMonSub::Move(_float _fDeltaTime)
{
	if (m_bMoveOn)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

		if (nullptr == pPlayerTransform)
			return E_FAIL;

		_vec3 vMonsterPos = m_pTransformCom[MONSUB_BASE]->Get_Desc().vPosition;
		_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

		_vec3 vLength = vPlayerPos - vMonsterPos;
		_vec3 vDir = vLength;
		D3DXVec3Normalize(&vDir, &vDir);
		float fLength = D3DXVec3Length(&vLength);

		if (fLength < 1.5f)
		{
			m_bMoveOn = false;
		}
		else if (m_bMoveOn)
		{
			vMonsterPos += vDir * (_fDeltaTime * 2.5f);
			m_pTransformCom[MONSUB_BASE]->Set_Position(vMonsterPos);
		}
	}
	return S_OK;
}
HRESULT CMonSub::Roll(_float _fDeltaTime)
{
	m_pTransformCom[MONSUB_RIGHT]->Turn(CTransform::AXIS_X, -_fDeltaTime);
	return S_OK;
}
CGameObject* CMonSub::Clone_GameObject(void * _pArg)
{
	CMonSub* pInstance = new CMonSub(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMonSub", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonSub::Free()
{
	for (_uint iCnt = 0; iCnt < MONSUB_END; ++iCnt)
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

CMonSub * CMonSub::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CMonSub* pInstance = new CMonSub(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMonSub", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMonSub::Setting_Dir()
{
	_vec3 vPos = m_pTransformCom[MONSUB_BASE]->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

	m_vDir = { m_vStartPos.x - vPos.x , 0.f , m_vStartPos.z - vPos.z };
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_bFallDown = true;
	m_bOnece = true;

	return S_OK;
}

HRESULT CMonSub::LookAtPlayer(_float _fDeltaTime)
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
	_vec3 vMonPos = m_pTransformCom[MONSUB_BASE]->Get_Desc().vPosition;
	_vec3 vMonLook = {};

	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&vMonLook, sizeof(_vec3), &m_pTransformCom[MONSUB_BASE]->Get_Desc().matWorld._31, sizeof(_vec3));

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

	if (fLimit > 0.2f)
		m_pTransformCom[MONSUB_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	else
		m_pTransformCom[MONSUB_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);

	return S_OK;
}
