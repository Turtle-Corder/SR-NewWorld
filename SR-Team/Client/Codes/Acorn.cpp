#include "stdafx.h"
#include "..\Headers\Acorn.h"
#include "DamageInfo.h"

USING(Client)

CAcorn::CAcorn(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CAcorn::CAcorn(const CAcorn& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CAcorn::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CAcorn::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CAcorn::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Update_State()))
		return GAMEOBJECT::WARN;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CAcorn::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CGameObject * CAcorn::Clone_GameObject(void * _pArg)
{
	CAcorn* pInstance = new CAcorn(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create CAcorn Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CAcorn::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

void CAcorn::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CAcorn * CAcorn::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CAcorn* pInstance = new CAcorn(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CAcorn", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CAcorn::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = m_tInstant.vPosition;
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.f , 1.f , 1.f };
	//-------------------------------------------------------

	//-------------------------------------------------------
	// Collider Setting
	//-------------------------------------------------------
	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.7f;
	//-------------------------------------------------------
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Stone", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Transform

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	// TODO : m_pStatusComp 셋팅
	CStatus::STAT tStat;
	tStat.iCriticalChance = 0;	tStat.iCriticalRate = 0;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 10;

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
		tDmgInfo.iCriticalRate = pOwnerStatusComp->Get_Status().iCriticalRate + m_pStatusCom->Get_Status().iCriticalRate;
		tDmgInfo.iCriticalChance = pOwnerStatusComp->Get_Status().iCriticalChance + m_pStatusCom->Get_Status().iCriticalChance;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
		tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
	}

	tDmgInfo.eType = NONE;


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAcorn::Movement(float _fDeltaTime)
{
	if (FAILED(FallDown_Acorn(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(IsOnTerrain(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Player_Position_Confirm()))
		return E_FAIL;

	if (FAILED(Move(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Attack(_fDeltaTime)))
		return E_FAIL;


	return S_OK;
}

HRESULT CAcorn::IsOnTerrain(_float _fDeltaTime)
{
	if (m_eCurState == CAcorn::MOVE)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;
	D3DXVECTOR3 vPosition = m_pTransformCom->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
	}

	if (vPosition.y > vPos.y)
	{
		m_eCurState = CAcorn::IDLE;
		m_pTransformCom->Set_Position(vPosition);
	}
	return S_OK;
}

HRESULT CAcorn::FallDown_Acorn(_float _fDeltaTime)
{
	if (m_eCurState != CAcorn::FALL)
		return S_OK;

	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

	m_fFallDownTime += _fDeltaTime;
	vPos.y -= m_fFallDownTime * 2.f;

	m_pTransformCom->Set_Position(_vec3(vPos.x, vPos.y, vPos.z));
	return S_OK;
}

HRESULT CAcorn::Player_Position_Confirm()
{
	if (m_eCurState != CAcorn::IDLE)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	m_vPrePos = pPlayerTransform->Get_Desc().vPosition;

	if (m_vPrePos != nullptr)
		m_eCurState = CAcorn::MOVE;


	return S_OK;
}

HRESULT CAcorn::Move(_float _fDeltaTime)
{
	if (m_eCurState != CAcorn::MOVE)
		return S_OK;

	_vec3	vMyPos = m_pTransformCom->Get_Desc().vPosition;
	_vec3	vDirection = m_vPrePos - vMyPos;
	_float	fDistance = D3DXVec3Length(&vDirection);
	D3DXVec3Normalize(&vDirection, &vDirection);

	if (fDistance <= 0.4f)
	{
		m_eCurState = CAcorn::ATTACK;
		return S_OK;
	}

	vMyPos += vDirection * (_fDeltaTime * 2.f);
	m_pTransformCom->Set_Position(vMyPos);

	if (m_eCurState == CAcorn::MOVE)
	{
		m_pTransformCom->Turn(CTransform::AXIS_Y, _fDeltaTime * 6.f);
	}

	return S_OK;
}

HRESULT CAcorn::Attack(_float _fDeltaTime)
{
	if (m_eCurState != CAcorn::ATTACK)
		return S_OK;


	// 여기서 터지고 나서 사망 데미지는 폭발로 처리??
	//m_bDead = true;
	/*
	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	_vec3 vMyPos = m_pTransformCom[STUMP_BASE]->Get_Desc().vPosition;
	tImpact.vPosition = vMyPos;
	*/
	//----------------------------------
	//----------------------------------
	// 여기서 터지는 공격같은것을 수행
	//----------------------------------
	return S_OK;
}

HRESULT CAcorn::Update_State()
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
		case STATE::FALL:
			break;
		}
		m_ePreState = m_eCurState;
	}

	return S_OK;
}

HRESULT CAcorn::Spawn_AcornExplosion(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	_vec3 vMyPos = m_pTransformCom->Get_Desc().vPosition;
	tImpact.vPosition = vMyPos;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_AcornExplosion",
		SCENE_STAGE0, LayerTag, &tImpact)))
		return E_FAIL;

	return S_OK;
}

