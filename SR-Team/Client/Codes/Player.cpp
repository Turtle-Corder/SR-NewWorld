#include "stdafx.h"
#include "Status.h"
#include "Inventory.h"
#include "DamageInfo.h"
#include "Shop.h"
#include "Equip.h"
#include "PlaneSkill.h"
#include "Skill.h"
#include "SkillInven.h"
#include "ItemInventory.h"
#include "Wand.h"
#include "Mouse.h"
#include "..\Headers\Player.h"

USING(Client)

//----------------------------------------------------------------------------------------------------
// Setup
//----------------------------------------------------------------------------------------------------
CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		m_pTextureCom[iCnt] = nullptr;
		m_pTransformCom[iCnt] = nullptr;
		m_pVIBufferCom[iCnt] = nullptr;
	}
}

CPlayer::CPlayer(const CPlayer& _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CPlayer::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	if (FAILED(Add_Wand(L"Layer_Wand")))
		return E_FAIL;


	m_fJumpPower = 5.f;
	m_fJumpTime = 0.f;

	m_tImpact.pAttacker = this;
	m_tImpact.pStatusComp = m_pStatusCom;

	return S_OK;
}



//----------------------------------------------------------------------------------------------------
// Common
//----------------------------------------------------------------------------------------------------
_int CPlayer::Update_GameObject(_float _fDeltaTime)
{

	//--------------------------------------------------
	// UI가 켜져있는 상태 -> 입력불가 처리
	//--------------------------------------------------
	if (Update_UICheck())
		return GAMEOBJECT::WARN;

	if (Update_Input(_fDeltaTime))
		return GAMEOBJECT::WARN;


	//--------------------------------------------------
	// 이동, 높이 보정
	//--------------------------------------------------
	if (FAILED(Update_Move(_fDeltaTime)))
		return GAMEOBJECT::WARN;


	//--------------------------------------------------
	// 상태, 애니메이션
	//--------------------------------------------------
	if (Update_State())
		return GAMEOBJECT::WARN;

	Update_Anim(_fDeltaTime);

	if (Update_Parts())
		return GAMEOBJECT::WARN;

	//--------------------------------------------------
	// 충돌체
	//--------------------------------------------------
	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom[PART_BODY]->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}



//----------------------------------------------------------------------------------------------------
// Render
//----------------------------------------------------------------------------------------------------
_int CPlayer::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagemnet = CManagement::Get_Instance();
	if (nullptr == pManagemnet)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagemnet->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	Update_AtkDelay(_fDeltaTime);

	return GAMEOBJECT::NOEVENT;
}

HRESULT CPlayer::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_uint iCnt = PART_START; iCnt < PART_END; ++iCnt)
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

HRESULT CPlayer::Take_Damage(const CComponent* _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;

	m_pStatusCom->Set_HP(((CDamageInfo*)_pDamageComp)->Get_Desc().iMinAtt);

	//if (0 >= m_pStatusCom->Get_Status().iHp)
	//	PRINT_LOG(L"아얏", LOG::CLIENT);

	return S_OK;
}

_bool CPlayer::IsOnBuff(ACTIVE_BUFF _eType)
{
	return m_bBuffActive[_eType];
}

void CPlayer::Buff_On(ACTIVE_BUFF _eType)
{
	switch (_eType)
	{
	case Client::CPlayer::BUFF_MANA:
		m_fConsumeRate = 0.f;
		break;

	case Client::CPlayer::BUFF_ATTACK:
		m_fAttackRate = 2.f;
		break;

	case Client::CPlayer::BUFF_SHIELD:
		break;

	default:
		break;
	}

	m_bBuffActive[_eType] = true;
}

void CPlayer::Buff_Off(ACTIVE_BUFF _eType)
{
	switch (_eType)
	{
	case Client::CPlayer::BUFF_MANA:
		m_fConsumeRate = 1.f;
		break;

	case Client::CPlayer::BUFF_ATTACK:
		m_fAttackRate = 1.f;
		break;

	case Client::CPlayer::BUFF_SHIELD:
		break;

	default:
		break;
	}

	m_bBuffActive[_eType] = false;
}

_int CPlayer::Get_ClearInfo()
{
	return m_iClearInfo;
}



//----------------------------------------------------------------------------------------------------
// Add Component
//----------------------------------------------------------------------------------------------------
HRESULT CPlayer::Add_Component()
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

HRESULT CPlayer::Add_Component_VIBuffer()
{
	//--------------------------------------------------
	// Clone
	//--------------------------------------------------
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		WCHAR szVIBuffer[MIN_STR] = L"";
		StringCchPrintf(szVIBuffer, _countof(szVIBuffer), L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szVIBuffer, (CComponent**)&m_pVIBufferCom[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer::Add_Component_Transform()
{
	CTransform::TRANSFORM_DESC tTransformDesc[PART_END];
	ZeroMemory(tTransformDesc, sizeof(CTransform::TRANSFORM_DESC) * PART_END);
	_float fRPS_Rad = D3DXToRadian(90.f);

	//--------------------------------------------------
	// HEAD
	//--------------------------------------------------
	tTransformDesc[PART_HEAD].vPosition = { 5.f, 5.f, 5.f };
	tTransformDesc[PART_HEAD].vScale = { 1.f, 1.f, 1.f };
	tTransformDesc[PART_HEAD].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_HEAD].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// BODY
	//--------------------------------------------------
	tTransformDesc[PART_BODY].vPosition = { tTransformDesc[PART_HEAD].vPosition + _vec3(0.f, -1.5f, 0.f) };
	tTransformDesc[PART_BODY].vScale = { 0.8f, 0.8f, 0.8f };
	tTransformDesc[PART_BODY].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_BODY].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// HAND_L
	//--------------------------------------------------
	tTransformDesc[PART_HAND_LEFT].vPosition = _vec3(-0.7f, 0.0f, 0.f);
	tTransformDesc[PART_HAND_LEFT].vScale = { 0.2f, 0.7f, 0.2f };
	tTransformDesc[PART_HAND_LEFT].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_HAND_LEFT].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// HAND_R
	//--------------------------------------------------
	tTransformDesc[PART_HAND_RIGHT].vPosition = _vec3(0.7f, 0.0f, 0.f);
	tTransformDesc[PART_HAND_RIGHT].vScale = { 0.2f, 0.7f, 0.2f };
	tTransformDesc[PART_HAND_RIGHT].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_HAND_RIGHT].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// FOOT_L
	//--------------------------------------------------
	tTransformDesc[PART_FOOT_LEFT].vPosition = _vec3(-0.2f, -0.8f, 0.f);
	tTransformDesc[PART_FOOT_LEFT].vScale = { 0.3f, 0.7f, 0.5f };
	tTransformDesc[PART_FOOT_LEFT].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_FOOT_LEFT].fRotatePerSecond = fRPS_Rad;

	//--------------------------------------------------
	// FOOT_R
	//--------------------------------------------------
	tTransformDesc[PART_FOOT_RIGHT].vPosition = _vec3(0.2f, -0.8f, 0.f);
	tTransformDesc[PART_FOOT_RIGHT].vScale = { 0.3f, 0.7f, 0.5f };
	tTransformDesc[PART_FOOT_RIGHT].fSpeedPerSecond = 5.f;
	tTransformDesc[PART_FOOT_RIGHT].fRotatePerSecond = fRPS_Rad;


	//--------------------------------------------------
	// Clone
	//--------------------------------------------------
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		WCHAR szTransform[MIN_STR] = L"";
		StringCchPrintf(szTransform, _countof(szTransform), L"Com_Transform%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szTransform, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer::Add_Component_Texture()
{
	WCHAR szTextureName[PART_END][MAX_PATH] =
	{
		// UNDONE : 텍스처 변경해야 함
		L"Component_Texture_Player",
		L"Component_Texture_PlayerHead",
		L"Component_Texture_PlayerHand",
		L"Component_Texture_PlayerHand",
		L"Component_Texture_PlayerFoot",
		L"Component_Texture_PlayerFoot"
	};

	//--------------------------------------------------
	// Clone
	//--------------------------------------------------
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		WCHAR szTexture[MIN_STR] = L"";
		StringCchPrintf(szTexture, _countof(szTexture), L"Com_Texture%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureName[iCnt], szTexture, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer::Add_Component_Extends()
{
	//--------------------------------------------------
	// Status
	//--------------------------------------------------
	CStatus::STAT tStat;
	tStat.iCriticalRate = 20;	tStat.iCriticalHit = 10;
	tStat.iDef = 50;
	tStat.iHp = 50;			tStat.iMp = 50;
	tStat.iMaxHp = 100;			tStat.iMaxMp = 100;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;
	tStat.iMaxFireStack = 3;
	tStat.iMaxIceStack = 3;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;


	//--------------------------------------------------
	// Collider
	//--------------------------------------------------
	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
	tColDesc.fRadius = 0.7f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;


	//--------------------------------------------------
	// Raycast
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Raycast", L"Com_Ray", (CComponent**)&m_pRaycastCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Add_Wand(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vPlayer_RightHand_Pos = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vPosition;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Wand", SCENE_ROOM, LayerTag, &vPlayer_RightHand_Pos)))
		return E_FAIL;

	return S_OK;
}



//----------------------------------------------------------------------------------------------------
// Create, Release
//----------------------------------------------------------------------------------------------------
CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CPlayer* pInstance = new CPlayer(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create Player", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject(void * _pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone Player", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	for (_uint i = 0; i < PART_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
	}

	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}



//----------------------------------------------------------------------------------------------------
// Movement
//----------------------------------------------------------------------------------------------------
HRESULT CPlayer::Update_Move(_float _fDeltaTime)
{
	//--------------------------------------------------
	// 이동 (x, z 이동)
	//--------------------------------------------------
	if (MOVE == m_eCurState)
	{
		Move_Target(_fDeltaTime);
	}

	//--------------------------------------------------
	// 점프
	//--------------------------------------------------
	//else if (JUMP == m_eCurState)
	//{
	//	Update_Jump(_fDeltaTime);
	//}


	//--------------------------------------------------
	// 높이 보정(y)
	//--------------------------------------------------
	if (IDLE == m_eCurState || MOVE == m_eCurState)
	{
		if (FAILED(Update_OnTerrain()))
			return E_FAIL;
	}

	return S_OK;
}

void CPlayer::Move_Target(_float _fDeltaTime)
{
	if (MOVE != m_eCurState)
		return;

	//--------------------------------------------------
	// 몸체의 위치를 기준으로 방향을 구한다. y축은 계산에서 제외
	//--------------------------------------------------
	_vec3 vCurPos = { m_pTransformCom[PART_BODY]->Get_Desc().vPosition.x, 0.f, m_pTransformCom[PART_BODY]->Get_Desc().vPosition.z };
	_vec3 vTargetNoneZ = { m_vTargetPos.x, 0.f, m_vTargetPos.z };

	_float fSpeedPerSecond = m_pTransformCom[PART_BODY]->Get_Desc().fSpeedPerSecond;

	_vec3 vMoveDir = vTargetNoneZ - vCurPos;
	_float fDist = D3DXVec3Length(&vMoveDir);

	if (m_fNearDist >= fDist)
		m_eCurState = IDLE;

	D3DXVec3Normalize(&vMoveDir, &vMoveDir);

	vCurPos = vMoveDir * fSpeedPerSecond * _fDeltaTime;

	m_pTransformCom[PART_HEAD]->Set_Position(m_pTransformCom[PART_HEAD]->Get_Desc().vPosition + vCurPos);
	m_pTransformCom[PART_BODY]->Set_Position(m_pTransformCom[PART_BODY]->Get_Desc().vPosition + vCurPos);
}

HRESULT CPlayer::Update_OnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	//--------------------------------------------------
	// 머리, 몸통 터레인 위로 조정.
	//--------------------------------------------------
	_vec3 vPosition = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
	if (pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom[PART_HEAD]->Set_Position(vPosition + _vec3(0.f, 1.3f, 0.f));
		m_pTransformCom[PART_BODY]->Set_Position(vPosition + _vec3(0.f, 0.5f, 0.f));
	}

	return S_OK;
}

HRESULT CPlayer::Update_Look(_float _fDeltaTime)
{
	// 플레이어의 전방
	_vec3 vLook = m_pTransformCom[PART_BODY]->Get_Look();
	D3DXVec3Normalize(&vLook, &vLook);

	// 타겟과의 방향
	_vec3 vPlayerToTarget = m_vTargetPos - m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
	D3DXVec3Normalize(&vPlayerToTarget, &vPlayerToTarget);

	// 각을 구함
	_float fDot = D3DXVec3Dot(&vLook, &vPlayerToTarget);;
	_float fRad = acosf(fDot);

	_vec3 vLeft;
	D3DXVec3Cross(&vLeft, &vLook, &_vec3(0.f, 1.f, 0.f));

	_float fLimit = D3DXVec3Dot(&vLeft, &vPlayerToTarget);
	if (fabsf(fLimit) < 0.3f)
		return S_OK;

	if (fLimit > 0)
	{
		m_pTransformCom[PART_HEAD]->Set_Rotation(m_pTransformCom[PART_HEAD]->Get_Desc().vRotate + _vec3(0.f, -fRad, 0.f));
		m_pTransformCom[PART_BODY]->Set_Rotation(m_pTransformCom[PART_BODY]->Get_Desc().vRotate + _vec3(0.f, -fRad, 0.f));
	}

	else
	{
		m_pTransformCom[PART_HEAD]->Set_Rotation(m_pTransformCom[PART_HEAD]->Get_Desc().vRotate + _vec3(0.f, fRad, 0.f));
		m_pTransformCom[PART_BODY]->Set_Rotation(m_pTransformCom[PART_BODY]->Get_Desc().vRotate + _vec3(0.f, fRad, 0.f));
	}


	return S_OK;
}



//----------------------------------------------------------------------------------------------------
// Raycast
//----------------------------------------------------------------------------------------------------
HRESULT CPlayer::Raycast_OnTerrain(_bool* _pFound)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	// 카메라의 월드행렬은 항등행렬!
	_matrix mat;
	D3DXMatrixIdentity(&mat);

	// 반환받을 마우스 위치값
	if (m_pRaycastCom->IsSimulate<VTX_TEXTURE, INDEX16>(g_hWnd, WINCX, WINCY, pTerrainBuffer, &mat, pCamera, &m_vTargetPos))
	{
		*_pFound = true;
		return S_OK;
	}

	return S_OK;
}



//----------------------------------------------------------------------------------------------------
// Common
//----------------------------------------------------------------------------------------------------
_int CPlayer::Update_UICheck()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return GAMEOBJECT::WARN;

	CShop* pShop = (CShop*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Shop");
	if (pShop == nullptr)
		return GAMEOBJECT::WARN;

	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return GAMEOBJECT::WARN;

	CSkill* pSkill = (CSkill*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 2);
	if (pSkill == nullptr)
		return GAMEOBJECT::WARN;

	if (pInven->Get_Render() || pShop->Get_Render() || pEquip->Get_Render() || pSkill->Get_Render())
		bShowUI = true;
	else
		bShowUI = false;

	return GAMEOBJECT::NOEVENT;
}

_int CPlayer::Update_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CPlayer::IDLE:
			m_iInputIdx_Slot = -1;
			m_iInputIdx_Anim = -1;
			break;
		case Client::CPlayer::MOVE:
			break;
			//case Client::CPlayer::JUMP:
			//	break;
		case Client::CPlayer::ATTACK:
			break;
		case Client::CPlayer::SKILL:
			break;
		default:
			break;
		}

		m_iAnimStep = 0;
		m_fAnimTimer = 0.f;
		m_ePreState = m_eCurState;
	}

	return GAMEOBJECT::NOEVENT;
}



//----------------------------------------------------------------------------------------------------
// Input
//----------------------------------------------------------------------------------------------------
_int CPlayer::Update_Input(_float _fDeltaTime)
{
	if (bShowUI)
		return GAMEOBJECT::NOEVENT;

	// Click & ETC key
	if (Update_Input_Action(_fDeltaTime))
		return GAMEOBJECT::WARN;

	// Z, Q, W, E, R, A, S, D, F key
	if (Update_Input_Skill(_fDeltaTime))
		return GAMEOBJECT::WARN;

	// 1, 2, 3, 4, 5, 6, 7, 8 key
	if (Update_Input_Item(_fDeltaTime))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

_int CPlayer::Update_Input_Action(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;


	//--------------------------------------------------
	// L Click : Move
	//--------------------------------------------------
	if (pManagement->Key_Pressing(VK_LBUTTON))
	{
		if (VALIDATE_MOVE >= m_eCurState)
		{
			_bool bFound = false;
			if (FAILED(Raycast_OnTerrain(&bFound)))
			{
				PRINT_LOG(L"Failed To Raycast!", LOG::CLIENT);
				return GAMEOBJECT::WARN;
			}

			// TODO : 길찾기로 대체


			if (bFound)
			{
				Update_Look(_fDeltaTime);
				m_eCurState = MOVE;
			}
		}
	}


	//--------------------------------------------------
	// R Click : 
	//--------------------------------------------------
	else if (pManagement->Key_Down(VK_RBUTTON))
	{
		// TODO : any..
	}


	//--------------------------------------------------
	// Space : Jump
	//--------------------------------------------------
	else if (pManagement->Key_Down(VK_SPACE))
	{
		//if(VALIDATE_MOVE >= m_eCurState)
		//	m_eCurState = JUMP;
	}

	//--------------------------------------------------
	// G : Interaction
	//--------------------------------------------------
	else if (pManagement->Key_Down('G'))
	{
		// TODO : Interaction
	}


	return GAMEOBJECT::NOEVENT;
}

_int CPlayer::Update_Input_Skill(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	CSkillInven* pSkillInven = (CSkillInven*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 3);
	if (pSkillInven == nullptr)
		return GAMEOBJECT::WARN;


	_int iRemainMP = 0;


	//--------------------------------------------------
	// Z : 일반 공격 (에너지 볼트)
	//--------------------------------------------------
	if (pManagement->Key_Down('Z'))
	{
		if (m_bCanNormalAtk && IDLE == m_eCurState || MOVE == m_eCurState)
		{
			m_bCanNormalAtk = false;
			m_eCurState = ATTACK;
		}
	}


	//--------------------------------------------------
	// Q 스킬 슬롯
	//--------------------------------------------------
	else if (pManagement->Key_Down('Q'))
	{
		if (IDLE == m_eCurState || MOVE == m_eCurState)
		{
			iRemainMP = m_pStatusCom->Get_Status().iMp - (_int)(pSkillInven->Get_ConsumeMP(SLOT_Q) * m_fConsumeRate);
			if (0 <= iRemainMP && pSkillInven->Can_UseSkill(SLOT_Q))
			{
				m_pStatusCom->Set_MP(iRemainMP);
				m_iInputIdx_Slot = SLOT_Q;
				m_iInputIdx_Anim = pSkillInven->Get_SkillID(SLOT_Q);
				m_eCurState = SKILL;
			}
		}
	}


	//--------------------------------------------------
	// W 스킬 슬롯
	//--------------------------------------------------
	else if (pManagement->Key_Down('W'))
	{
		if (IDLE == m_eCurState || MOVE == m_eCurState)
		{
			iRemainMP = m_pStatusCom->Get_Status().iMp - (_int)(pSkillInven->Get_ConsumeMP(SLOT_W) * m_fConsumeRate);
			if (0 <= iRemainMP && pSkillInven->Can_UseSkill(SLOT_W))
			{
				m_pStatusCom->Set_MP(iRemainMP);
				m_iInputIdx_Slot = SLOT_W;
				m_iInputIdx_Anim = pSkillInven->Get_SkillID(SLOT_W);
				m_eCurState = SKILL;
			}
		}
	}


	//--------------------------------------------------
	// E 스킬 슬롯
	//--------------------------------------------------
	else if (pManagement->Key_Down('E'))
	{
		if (IDLE == m_eCurState || MOVE == m_eCurState)
		{
			iRemainMP = m_pStatusCom->Get_Status().iMp - (_int)(pSkillInven->Get_ConsumeMP(SLOT_E) * m_fConsumeRate);
			if (0 <= iRemainMP && pSkillInven->Can_UseSkill(SLOT_E))
			{
				m_pStatusCom->Set_MP(iRemainMP);
				m_iInputIdx_Slot = SLOT_E;
				m_iInputIdx_Anim = pSkillInven->Get_SkillID(SLOT_E);
				m_eCurState = SKILL;
			}
		}
	}


	//--------------------------------------------------
	// R 스킬 슬롯
	//--------------------------------------------------
	else if (pManagement->Key_Down('R'))
	{
		if (IDLE == m_eCurState || MOVE == m_eCurState)
		{
			iRemainMP = m_pStatusCom->Get_Status().iMp - (_int)(pSkillInven->Get_ConsumeMP(SLOT_R) * m_fConsumeRate);
			if (0 <= iRemainMP && pSkillInven->Can_UseSkill(SLOT_R))
			{
				m_pStatusCom->Set_MP(iRemainMP);
				m_iInputIdx_Slot = SLOT_R;
				m_iInputIdx_Anim = pSkillInven->Get_SkillID(SLOT_R);
				m_eCurState = SKILL;
			}
		}
	}


	//--------------------------------------------------
	// A 스킬 슬롯
	//--------------------------------------------------
	else if (pManagement->Key_Down('A'))
	{
		if (IDLE == m_eCurState || MOVE == m_eCurState)
		{
			iRemainMP = m_pStatusCom->Get_Status().iMp - (_int)(pSkillInven->Get_ConsumeMP(SLOT_A) * m_fConsumeRate);
			if (0 <= iRemainMP && pSkillInven->Can_UseSkill(SLOT_A))
			{
				m_pStatusCom->Set_MP(iRemainMP);
				m_iInputIdx_Slot = SLOT_A;
				m_iInputIdx_Anim = pSkillInven->Get_SkillID(SLOT_A);
				m_eCurState = SKILL;
			}
		}
	}


	//--------------------------------------------------
	// S 스킬 슬롯
	//--------------------------------------------------
	else if (pManagement->Key_Down('S'))
	{
		if (IDLE == m_eCurState || MOVE == m_eCurState)
		{
			iRemainMP = m_pStatusCom->Get_Status().iMp - (_int)(pSkillInven->Get_ConsumeMP(SLOT_S) * m_fConsumeRate);
			if (0 <= iRemainMP && pSkillInven->Can_UseSkill(SLOT_S))
			{
				m_pStatusCom->Set_MP(iRemainMP);
				m_iInputIdx_Slot = SLOT_S;
				m_iInputIdx_Anim = pSkillInven->Get_SkillID(SLOT_S);
				m_eCurState = SKILL;
			}
		}
	}


	//--------------------------------------------------
	// D 스킬 슬롯
	//--------------------------------------------------
	else if (pManagement->Key_Down('D'))
	{
		if (IDLE == m_eCurState || MOVE == m_eCurState)
		{
			iRemainMP = m_pStatusCom->Get_Status().iMp - (_int)(pSkillInven->Get_ConsumeMP(SLOT_D) * m_fConsumeRate);
			if (iRemainMP <= 0 && pSkillInven->Can_UseSkill(SLOT_D))
			{
				m_pStatusCom->Set_MP(iRemainMP);
				m_iInputIdx_Slot = SLOT_D;
				m_iInputIdx_Anim = pSkillInven->Get_SkillID(SLOT_D);
				m_eCurState = SKILL;
			}
		}
	}


	//--------------------------------------------------
	// F 스킬 슬롯
	//--------------------------------------------------
	else if (pManagement->Key_Down('F'))
	{
		if (IDLE == m_eCurState || MOVE == m_eCurState)
		{
			iRemainMP = m_pStatusCom->Get_Status().iMp - (_int)(pSkillInven->Get_ConsumeMP(SLOT_F) * m_fConsumeRate);
			if (iRemainMP <= 0 && pSkillInven->Can_UseSkill(SLOT_F))
			{
				m_pStatusCom->Set_MP(iRemainMP);
				m_iInputIdx_Slot = SLOT_F;
				m_iInputIdx_Anim = pSkillInven->Get_SkillID(SLOT_F);
				m_eCurState = SKILL;
			}
		}
	}

	// 아무것도 안누르면 변화 없음

	return GAMEOBJECT::NOEVENT;
}

_int CPlayer::Update_Input_Item(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;
	CItemInventory* pItemInven = (CItemInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 4);
	if (pItemInven == nullptr)
		return GAMEOBJECT::WARN;

	if (pManagement->Key_Down('1'))
	{
		pItemInven->Actual_UseItem(SLOT_1);
	}
	else if (pManagement->Key_Down('2'))
	{
		pItemInven->Actual_UseItem(SLOT_2);
	}
	else if (pManagement->Key_Down('3'))
	{
		pItemInven->Actual_UseItem(SLOT_3);
	}
	else if (pManagement->Key_Down('4'))
	{
		pItemInven->Actual_UseItem(SLOT_4);
	}

	return GAMEOBJECT::NOEVENT;
}


_bool CPlayer::Actual_UseSkill()
{
	if (SKILL != m_eCurState)
		return false;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	CSkillInven* pSkillInven = (CSkillInven*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 3);
	if (nullptr == pSkillInven)
		return false;

	if (!pSkillInven->Actual_UseSkill(m_iInputIdx_Slot, (void*)&m_tImpact))
		return false;

	return true;
}

_int CPlayer::Update_Parts()
{
	//--------------------------------------------------
	// 공전값 구함
	//--------------------------------------------------
	_matrix matRevolution;
	_vec3 vRotate = m_pTransformCom[PART_BODY]->Get_Desc().vRotate;
	D3DXMatrixIdentity(&matRevolution);
	D3DXMatrixRotationZ(&matRevolution, vRotate.z);
	D3DXMatrixRotationY(&matRevolution, vRotate.y);
	D3DXMatrixRotationX(&matRevolution, vRotate.x);


	//--------------------------------------------------
	// 부모값 구함
	//--------------------------------------------------
	_matrix matParent = m_pTransformCom[PART_BODY]->Get_Desc().matWorld;

	_vec3 vRelativeHand = { 0.f, -1.f, 0.f };
	_vec3 vRelativeFoot = { 0.f, -1.5f, 0.f };

	HRESULT hr = 0;

	//--------------------------------------------------
	// 스케일 * 자전 * 이동
	//--------------------------------------------------
	hr = m_pTransformCom[PART_HEAD]->Update_Transform();
	hr = m_pTransformCom[PART_BODY]->Update_Transform();


	//--------------------------------------------------
	// 스케일 * 자전 * 이동 * 공전 * 부모
	//--------------------------------------------------
	hr = m_pTransformCom[PART_HAND_LEFT]->Update_Transform(matRevolution, matParent);
	hr = m_pTransformCom[PART_HAND_RIGHT]->Update_Transform(matRevolution, matParent);
	hr = m_pTransformCom[PART_FOOT_LEFT]->Update_Transform(matRevolution, matParent);
	hr = m_pTransformCom[PART_FOOT_RIGHT]->Update_Transform(matRevolution, matParent);


	if (FAILED(hr))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

//void CPlayer::Update_Jump(_float fDeltaTime)
//{
//	if (JUMP != m_eCurState)
//		return;
//
//	// 플레이어의 현재 위치를 계속 받아온다
//	_vec3 vCurPos[2];
//	for (_uint i = 0; i < 2; ++i)
//		vCurPos[i] = m_pTransformCom[i]->Get_Desc().vPosition;
//
//	// 시간 더해주기
//	m_fJumpTime += fDeltaTime;
//	for (_uint i = 0; i < 2; ++i)
//	{
//		// 점프
//		vCurPos[i].y += (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
//
//		if (vCurPos[i].y < m_pTransformCom[i]->Get_Desc().vPosition.y)
//		{
//			m_fJumpTime = 0.f;
//			vCurPos[i].y = m_pTransformCom[i]->Get_Desc().vPosition.y;
//			m_eCurState = IDLE;
//		}
//
//		m_pTransformCom[i]->Set_Position(vCurPos[i]);
//	}
//}

void CPlayer::Update_Anim(_float _fDeltaTime)
{
	switch (m_eCurState)
	{
	case Client::CPlayer::IDLE:
	case Client::CPlayer::MOVE:
		Update_Anim_Move(_fDeltaTime);
		break;

		//case Client::CPlayer::JUMP:
		//	break;

	case Client::CPlayer::ATTACK:
		Update_Anim_Attack(_fDeltaTime);
		break;

	case Client::CPlayer::SKILL:
		Update_Anim_Skill(_fDeltaTime);
		break;

	default:
		break;
	}
}

void CPlayer::Update_Anim_Move(_float _fDeltaTime)
{
	if (MOVE == m_eCurState)
	{
		m_fAnimTimer += _fDeltaTime;

		if (m_fAnimTimer >= 0.5f)
		{
			if (m_iAnimStep)	m_iAnimStep = 0;
			else				m_iAnimStep = 1;

			m_fAnimTimer = 0.f;

			for (_uint i = 2; i < PART_END; ++i)
				m_pTransformCom[i]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		}

		//--------------------------------------------------
		// 회전 (자전)
		//--------------------------------------------------
		if (m_iAnimStep)
		{
			m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, _fDeltaTime);
			m_pTransformCom[PART_HAND_LEFT]->Turn(CTransform::AXIS_X, -_fDeltaTime);

			m_pTransformCom[PART_FOOT_RIGHT]->Turn(CTransform::AXIS_X, -_fDeltaTime);
			m_pTransformCom[PART_FOOT_LEFT]->Turn(CTransform::AXIS_X, _fDeltaTime);
		}

		else
		{
			m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -_fDeltaTime);
			m_pTransformCom[PART_HAND_LEFT]->Turn(CTransform::AXIS_X, _fDeltaTime);

			m_pTransformCom[PART_FOOT_RIGHT]->Turn(CTransform::AXIS_X, _fDeltaTime);
			m_pTransformCom[PART_FOOT_LEFT]->Turn(CTransform::AXIS_X, -_fDeltaTime);
		}
	}

	//--------------------------------------------------
	// 멈춤
	//--------------------------------------------------
	else if (IDLE == m_eCurState)
	{
		for (_uint i = 2; i < PART_END; ++i)
			m_pTransformCom[i]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	}
}

void CPlayer::Update_Anim_Attack(_float _fDeltaTime)
{
	if (ATTACK != m_eCurState)
		return;

	m_fAnimTimer += _fDeltaTime;
	if (0 == m_iAnimStep)
	{
		m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -_fDeltaTime * 5.f);

		if (m_fAnimTimer >= 0.2f)
		{
			m_fAnimTimer = 0.f;
			++m_iAnimStep;

			Spawn_EnergyBolt();
		}
	}

	else if (1 == m_iAnimStep)
	{
		m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Y, _fDeltaTime * 5.f);

		if (m_fAnimTimer >= 0.2f)
		{
			m_fAnimTimer = 0.f;
			++m_iAnimStep;
		}
	}

	else if (2 == m_iAnimStep)
	{
		m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Y, -_fDeltaTime * 5.f);

		if (m_fAnimTimer >= 0.3f)
		{
			m_pTransformCom[PART_HAND_RIGHT]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_eCurState = IDLE;
		}
	}
}

void CPlayer::Update_Anim_Skill(_float _fDeltaTime)
{
	if (SKILL != m_eCurState)
		return;

	// 일단 하나로 통일
	switch ((eActiveSkill_ID)m_iInputIdx_Anim)
	{
	case ACTIVE_ICE_BLAST:
	case ACTIVE_MANA_DRIFT:
	case ACTIVE_ENERGY_EXPLOTIATION:
	case ACTIVE_FLAME_WAVE:
	case ACTIVE_ICE_SPEAR:
	case ACTIVE_MAGIC_ARMOR:
	case ACTIVE_THUNDER_STORM:
	case ACTIVE_BLIND:
		Update_Anim_Skill_Common(_fDeltaTime);
		break;
	default:
		break;
	}
}

void CPlayer::Update_Anim_Skill_Common(_float _fDeltaTime)
{
	m_fAnimTimer += _fDeltaTime;
	if (0 == m_iAnimStep)
	{
		m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -_fDeltaTime * 5.f);

		if (m_fAnimTimer >= 0.2f)
		{
			m_fAnimTimer = 0.f;
			++m_iAnimStep;

			Actual_UseSkill();
		}
	}

	else if (1 == m_iAnimStep)
	{
		if (m_fAnimTimer >= 0.4f)
		{
			m_fAnimTimer = 0.f;
			++m_iAnimStep;
		}
	}

	else if (2 == m_iAnimStep)
	{
		m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, _fDeltaTime * 5.f);

		if (m_fAnimTimer >= 0.2f)
		{
			m_pTransformCom[PART_HAND_RIGHT]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
			m_eCurState = IDLE;
		}
	}
}

HRESULT CPlayer::Spawn_EnergyBolt()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;

	_vec3 vWandPos = {};
	memcpy_s(&vWandPos, sizeof(_vec3), &m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().matWorld._41, sizeof(_vec3));
	_vec3 vPlayerLook = m_pTransformCom[PART_HEAD]->Get_Look();
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	tImpact.vPosition = vWandPos;
	tImpact.vDirection = vPlayerLook;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_EnergyBolt", pManagement->Get_CurrentSceneID(), L"Layer_PlayerAtk", &tImpact)))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Update_AtkDelay(_float _fDeltaTime)
{
	if (!m_bCanNormalAtk)
	{
		m_fAtkDelayCounter += _fDeltaTime;
		if (m_fAtkDelayCounter > m_fAtkDelayTime)
		{
			m_bCanNormalAtk = true;
			m_fAtkDelayCounter = 0.f;
		}
	}
}
