#include "stdafx.h"
#include "..\Headers\Bomb.h"
#include "DamageInfo.h"
#include "MainCamera.h"
#include "Sound_Manager.h"
USING(Client)

CBomb::CBomb(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CBomb::CBomb(const CBomb& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CBomb::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBomb::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBomb::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(IsOnTerrain(_fDeltaTime)))
		return GAMEOBJECT::WARN;


	if (FAILED(Dead_Bomb(_fDeltaTime)))
		return GAMEOBJECT::WARN;


	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	if (m_iTexCnt == 2) // 터지는 상황
	{
		m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition);
	}
	else
		m_pColliderCom->Update_Collider(_vec3(99.f, 99.f, 99.f));

	return GAMEOBJECT::NOEVENT;
}

_int CBomb::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CBomb::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(m_iTexCnt)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBomb::Add_Component()
{
	//--------------------------------------------------
	// VIBuffer Component
	//--------------------------------------------------
	if (FAILED(Add_Component_VIBuffer()))
		return E_FAIL;

	//--------------------------------------------------
	// Transform Component
	//--------------------------------------------------
	if (FAILED(Add_Component_Transform()))
		return E_FAIL;



	//--------------------------------------------------
	// Texture Component
	//--------------------------------------------------
	if (FAILED(Add_Component_Texture()))
		return E_FAIL;



	//--------------------------------------------------
	// Extends Component
	//--------------------------------------------------
	if (FAILED(Add_Component_Extends()))
		return E_FAIL;



	return S_OK;
}

HRESULT CBomb::Add_Component_VIBuffer()
{
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBomb::Add_Component_Transform()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vPosition = m_tInstant.vPosition;
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.f , 0.5f , 1.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBomb::Add_Component_Texture()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_VOLCANIC, L"Component_Texture_Bomb", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBomb::Add_Component_Extends()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = m_pTransformCom->Get_Desc().vPosition;
	tColDesc.fRadius = 0.7f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;


	CStatus::STAT tStat;
	tStat.iCriticalChance = 0;	tStat.iCriticalRate = 0;
	tStat.iMinAtt = 20;			tStat.iMaxAtt = 20;
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


HRESULT CBomb::IsOnTerrain(_float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	_vec3 vPosition = m_pTransformCom->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom->Set_Position(vPosition);
	}

	return S_OK;
}

CGameObject* CBomb::Clone_GameObject(void * _pArg)
{
	CBomb* pInstance = new CBomb(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CBomb", LOG::CLIENT);
		Safe_Release(pInstance);
	}



	return pInstance;
}

void CBomb::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CBomb * CBomb::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CBomb* pInstance = new CBomb(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CBomb", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CBomb::Dead_Bomb(_float _fDeltaTime)
{
	m_fBombTime += _fDeltaTime;

	if (m_fBombTime >= 0.4f)
	{
		m_fBombTime = 0.f;
		++m_iAnimationStep;

		if (6 == m_iAnimationStep)
		{
			m_bDead = true;
			CSoundManager::Get_Instance()->PlayEffect(L"BombExplosion.mp3");
			Make_Bomb_Residue();
			return S_OK;
		}
	}

	if (m_iAnimationStep >= 5)
	{
		m_iTexCnt = 2;

		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		CMainCamera* pCamera = (CMainCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
		if (nullptr == pCamera)
			return E_FAIL;

		pCamera->Set_Camera_Wigging(0.7f, 100.f, 1.f, CMainCamera::WIG_TYPE::HARMONIC);



		m_pTransformCom->Set_Scale(_vec3(3.f, 3.f, 3.f));
	}
	else if (0 == m_iAnimationStep % 2)
		m_iTexCnt = 0;
	else if (1 == m_iAnimationStep % 2)
		m_iTexCnt = 1;


	return S_OK;
}

HRESULT CBomb::Make_Bomb_Residue()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;

	for (_uint i = 0; i < 12; i++)
	{
		_vec3 RandomPostion = { (_float)(rand() % 30 - 15), 18.f + (_float)(rand() % 4 - 2) ,(_float)(rand() % 30 - 15) };

		tImpact.vPosition = m_pTransformCom->Get_Desc().vPosition;
		tImpact.vDirection = RandomPostion;
		tImpact.vOption = RandomPostion + m_pTransformCom->Get_Desc().vPosition;
	

		if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_Bomb_Residue", pManagement->Get_CurrentSceneID(), L"Layer_Effect", &tImpact)))
			return E_FAIL;
	}

	return E_NOTIMPL;
}