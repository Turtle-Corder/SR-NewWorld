#include "stdafx.h"
#include "..\Headers\Slime.h"
#include "DamageInfo.h"

USING(Client)

CSlime::CSlime(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CSlime::CSlime(const CSlime & _rOther)
	: CGameObject(_rOther)
{
	m_bIsClone = true;
}

HRESULT CSlime::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSlime::Setup_GameObject(void * pArg)
{
	m_bDead = false;

	if (pArg)
		m_tSlimeInfo = *(SLIMEINFO*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CSlime::Update_GameObject(_float _fDeltaTime)
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (m_bDead)
	{
		////-------------------------------------------------
		//// 1 => 2 => 4 => Release;
		////-------------------------------------------------
		if (m_bIsClone && m_iCurCount < m_iMaxCount)
		{
			m_iCurCount *= 2;

			for (int i = 0; i < m_iCurCount; ++i)
			{
				if (FAILED(Divide_Cube(L"Layer_Monster")))
					return GAMEOBJECT::WARN;
			}
		}
		else
		{
			//if (FAILED(Create_Item(L"Layer_DropItem")))
			//return GAMEOBJECT::WARN;
		}

		return GAMEOBJECT::DEAD;
	}

	if (FAILED(Update_State()))
		return E_FAIL;

	if (pManagement->Key_Down(VK_F4))
		m_bDead = true;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;



	if (pManagement->Key_Down(VK_F8))
		m_eCurState = CSlime::MOVE;

	if (FAILED(Setting_SlimeJelly()))
		return GAMEOBJECT::WARN;

	if (FAILED(Setting_SlimeBody()))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CSlime::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CSlime::Render_BlendAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[SLIME_JELLY]->Set_Transform(&m_pTransformCom[SLIME_JELLY]->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(SLIME_JELLY)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[SLIME_JELLY]->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[SLIME_BODY]->Set_Transform(&m_pTransformCom[SLIME_BODY]->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(SLIME_BODY)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[SLIME_BODY]->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;

	//m_pStatusCom->Set_HP(((CDamageInfo*)_pDamageComp)->Get_Desc().iMinAtt);

	//if (0 >= m_pStatusCom->Get_Status().iHp)
	//{
		m_bDead = true;
	//}

	return S_OK;
}

HRESULT CSlime::Add_Component()
{
	m_iCurCount = m_tSlimeInfo.iCurCount;




	CTransform::TRANSFORM_DESC tTransformDesc[SLIME_END];

	tTransformDesc[SLIME_BODY].vPosition = { 0.f , 0.f , 0.01f };
	tTransformDesc[SLIME_BODY].fSpeedPerSecond = 10.f;
	tTransformDesc[SLIME_BODY].fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc[SLIME_BODY].vScale = { 1.0f / (float)m_iCurCount , 1.0f / (float)m_iCurCount, 1.0f / (float)m_iCurCount };

	tTransformDesc[SLIME_JELLY].vPosition = { m_tSlimeInfo.vPos.x , 0.f, m_tSlimeInfo.vPos.z };
	tTransformDesc[SLIME_JELLY].fSpeedPerSecond = 10.f;
	tTransformDesc[SLIME_JELLY].fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc[SLIME_JELLY].vScale = { 1.4f / (float)m_iCurCount , 1.4f / (float)m_iCurCount , 1.4f / (float)m_iCurCount };

	CSphereCollider::COLLIDER_DESC tCollDesc;
	tCollDesc.vPosition = tTransformDesc[SLIME_BASE].vPosition;
	tCollDesc.fRadius = 0.5f/(_float)m_iCurCount;


	//--------------------------------------------------
	// Body Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer1", (CComponent**)&m_pVIBufferCom[SLIME_BODY])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform1", (CComponent**)&m_pTransformCom[SLIME_BODY], &tTransformDesc[SLIME_BODY])))
		return E_FAIL;

	CStatus::STAT tStat;
	tStat.iCriticalChance = 20;	tStat.iCriticalRate = 10;
	tStat.iDef = 50;
	tStat.iHp = 100;			tStat.iMp = 100;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	//--------------------------------------------------
	// Jelly Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer0", (CComponent**)&m_pVIBufferCom[SLIME_JELLY])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform0", (CComponent**)&m_pTransformCom[SLIME_JELLY], &tTransformDesc[SLIME_JELLY])))
		return E_FAIL;


	//--------------------------------------------------
	// Texture Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE1, L"Component_Texture_Slime", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	m_ePreState = CSlime::IDLE;
	m_eCurState = CSlime::IDLE;

	CDamageInfo::DAMAGE_DESC tDmgInfo;
	tDmgInfo.iMinAtt = m_pStatusCom->Get_Status().iMinAtt;
	tDmgInfo.iMaxAtt = m_pStatusCom->Get_Status().iMaxAtt;
	tDmgInfo.iCriticalChance = m_pStatusCom->Get_Status().iCriticalChance;
	tDmgInfo.iCriticalRate = m_pStatusCom->Get_Status().iCriticalRate;
	tDmgInfo.pOwner = this;
	tDmgInfo.eType = eELEMENTAL_TYPE::NONE;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tCollDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (FAILED(LookAtPlayer(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Compare_PlayerPosition()))
		return E_FAIL;

	if (FAILED(Jumping(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Move(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(Attack(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPosition = m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom[SLIME_BASE]->Set_Position(vPosition);

		if (m_eCurState == STATE::MOVE)
		{
			m_bJump = true;
		}
	}

	return S_OK;
}

HRESULT CSlime::Jumping(float _fDeltaTime)
{
	if (m_eCurState == CSlime::IDLE || m_eCurState == CSlime::MOVE)
	{
		D3DXVECTOR3 vPos = m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition;

		if (m_bJump)
		{
			vPos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f;
			m_fJumpTime += _fDeltaTime;

			if (vPos.y < m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition.y)
			{
				m_bJump = false;
				m_fJumpTime = 0.f;
			}
			m_pTransformCom[SLIME_BASE]->Set_Position(vPos);
		}
	}

	return S_OK;
}

HRESULT CSlime::LookAtPlayer(float _fDeltaTime)
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
	_vec3 vMonPos = m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition;
	_vec3 vMonLook = {};

	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&vMonLook, sizeof(_vec3), &m_pTransformCom[SLIME_BASE]->Get_Desc().matWorld._31, sizeof(_vec3));

	_vec3 vMonToPlayer = vPlayerPos - vMonPos;

	D3DXVec3Normalize(&vMonLook, &vMonLook);
	D3DXVec3Normalize(&vMonToPlayer, &vMonToPlayer);

	float fDot = 0.f;
	float fRad = 0.f;

	fDot = D3DXVec3Dot(&vMonLook, &vMonToPlayer);
	fRad = (float)acos(fDot);

	_vec3 vMonRight = {};
	D3DXVec3Cross(&vMonRight, &vMonLook, &_vec3(0.f, 1.f, 0.f));

	D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	float fLimit = D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	if (fabsf(fLimit) < 0.2f)
		return S_OK;

	if (fLimit > 0)
	{
		m_pTransformCom[SLIME_BASE]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	}
	else
	{
		m_pTransformCom[SLIME_BASE]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
	}

	//------------------------------------------
	// 공전 구현 순서생각하기 LateUpdate안에 월드구성하고 나서
	//------------------------------------------
	/*	D3DXMATRIX matTrans , matRevY , matParent;
	D3DXMatrixTranslation(&matTrans, 2.f, 0.f, 2.f);
	D3DXMatrixRotationY(&matRevY, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matParent, vMonPos.x, vMonPos.y, vMonPos.z);
	m_pTransformCom->Set_WorldMatrix(matTrans * matRevY * matParent);*/
	//------------------------------------------
	return S_OK;
}

HRESULT CSlime::Compare_PlayerPosition()
{
	if (m_eCurState != CSlime::IDLE)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPosition = pPlayerTransform->Get_Desc().vPosition;
	_vec3 vMyPos = m_pTransformCom[SLIME_JELLY]->Get_Desc().vPosition;
	_vec3 vDirection = _vec3(vPlayerPosition.x, 0.f, vPlayerPosition.z) - _vec3(vMyPos.x, 0.f, vMyPos.z);
	_float fLength = D3DXVec3Length(&vDirection);

	if (fLength < 4.f)
		m_eCurState = CSlime::MOVE;

	return S_OK;
}

HRESULT CSlime::Divide_Cube(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vMyPos = m_pTransformCom[SLIME_JELLY]->Get_Desc().vPosition;
	SLIMEINFO tInfo;
	tInfo.vPos = vMyPos + _vec3{ (_float)m_iCurCount *(rand()%4), 0.f , (_float)m_iCurCount *(rand()%4) };
	tInfo.iCurCount = m_iCurCount;
	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Slime", pManagement->Get_CurrentSceneID(), LayerTag, &tInfo)))
		return E_FAIL;

	return S_OK;
}

CSlime * CSlime::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSlime* pInstance = new CSlime(pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CTranslucentCube", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSlime::Clone_GameObject(void * pArg)
{
	CSlime* pInstance = new CSlime(*this);

	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CTranslucentCube", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSlime::Free()
{
	for (_uint iCnt = 0; iCnt < SLIME_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
	}

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

HRESULT CSlime::Spawn_Item(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vPos = m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_DropItem", pManagement->Get_CurrentSceneID(), LayerTag, &vPos)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::Spawn_Crack(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vPos = m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Crack", pManagement->Get_CurrentSceneID(), LayerTag, &vPos)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::Spawn_InstantImpact(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	_vec3 BodyPos = {};
	memcpy_s(&BodyPos, sizeof(_vec3), &m_pTransformCom[SLIME_BASE]->Get_Desc().matWorld._41, sizeof(_vec3));
	tImpact.vPosition = BodyPos;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Slime_Impact", pManagement->Get_CurrentSceneID(), LayerTag, &tImpact)))/*여기 StartPos*/
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::Setting_SlimeBody()
{
	m_pTransformCom[SLIME_BODY]->Set_Position(m_pTransformCom[SLIME_JELLY]->Get_Desc().vPosition - _vec3(0.f, 0.f, 0.01f));
	m_pTransformCom[SLIME_BODY]->Set_Rotation(m_pTransformCom[SLIME_JELLY]->Get_Desc().vRotate);

	m_pTransformCom[SLIME_BODY]->Update_Transform();

	return S_OK;
}

HRESULT CSlime::Setting_SlimeJelly()
{
	if (FAILED(m_pTransformCom[SLIME_JELLY]->Update_Transform()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::Move(_float _fDeltaTime)
{
	if (m_eCurState != CSlime::MOVE)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 m_vPos = m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition;
	_vec3 vDir = vPlayerPos - m_vPos;
	m_fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	if (1.f <= m_fDistance)
	{
		m_vPos += vDir * _fDeltaTime * 3.f;
		m_pTransformCom[SLIME_BASE]->Set_Position(m_vPos);
	}
	else if (1.f >= m_fDistance)
	{
		m_eCurState = CSlime::ATTACK;
		m_bAttackJump = true;
	}

	return S_OK;
}

HRESULT CSlime::Update_State()
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
		m_ePreState = m_eCurState;
	}

	return S_OK;
}

HRESULT CSlime::Attack(_float _fDeltaTime)
{
	if (m_eCurState != CSlime::ATTACK)
		return S_OK;


	if (m_eCurState == CSlime::ATTACK)
	{
		m_fStartTime += _fDeltaTime;

		D3DXVECTOR3 vPos = m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition;

		if (m_fStartTime > 0.5f)
		{
			if (m_bJump)
			{
				vPos.y += (m_fJumpPower * 2.f) * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f;
				m_fJumpTime += _fDeltaTime;

				if (vPos.y < m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition.y)
				{
					vPos.y = m_pTransformCom[SLIME_BASE]->Get_Desc().vPosition.y;
					m_bAttackJump = false;
					m_fJumpTime = 0.f;
					m_fStartTime = 0.f;

					if (FAILED(Spawn_Crack(L"Layer_Effect")))
						return E_FAIL;

					if (FAILED(Spawn_InstantImpact(L"Layer_MonsterAtk")))
						return E_FAIL;

					m_eCurState = CSlime::MOVE;
				}

				m_pTransformCom[SLIME_BASE]->Set_Position(vPos);
			}
		}
	}


	return S_OK;
}