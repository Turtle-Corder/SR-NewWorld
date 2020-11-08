#include "stdafx.h"
#include "DamageInfo.h"
#include "..\Headers\MiniGolem.h"

USING(Client)

CMiniGolem::CMiniGolem(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CMiniGolem::CMiniGolem(const CMiniGolem & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CMiniGolem::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMiniGolem::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	Set_DeActive();

	return S_OK;
}

_int CMiniGolem::Update_GameObject(_float _fDeltaTime)
{
	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	Update_Animation_Attack(_fDeltaTime);
	IsOnTerrain();

	m_pTransformCom[MINIGOLEM_BASE]->Update_Transform();
	for (_uint iCnt = 1; iCnt < MINIGOLEM_END; ++iCnt)
		m_pTransformCom[iCnt]->Update_Transform(m_pTransformCom[MINIGOLEM_BASE]->Get_Desc().matWorld);

	return GAMEOBJECT::NOEVENT;
}

_int CMiniGolem::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	Update_Disappear(_fDeltaTime);

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMiniGolem::Render_NoneAlpha()
{
	if (!m_bActive)
		return S_OK;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int iCnt = MINIGOLEM_BODY; iCnt < MINIGOLEM_END; ++iCnt)
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

HRESULT CMiniGolem::Add_Component()
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

HRESULT CMiniGolem::Add_Component_VIBuffer()
{
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint iCnt = 0; iCnt < MINIGOLEM_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), L"Com_VIBuffer%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szCombine, (CComponent**)&m_pVIBufferCom[iCnt]))) //»ý¼º °¹¼ö
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMiniGolem::Add_Component_Transform()
{
	_float fSpeedPerSecond = 10.f;
	_float fRotatePerSecond = D3DXToRadian(90.f);

	CTransform::TRANSFORM_DESC tTransformDesc[MINIGOLEM_END] = {};

	tTransformDesc[MINIGOLEM_BASE].vPosition = { 999.f, 999.f, 999.f };//m_tImpact.vPosition;

	tTransformDesc[MINIGOLEM_BODY].vPosition = { 0.f , 1.f, 0.f };
	tTransformDesc[MINIGOLEM_BODY].vScale = { 1.0f , 1.f , 0.5f };

	tTransformDesc[MINIGOLEM_HEAD].vPosition = { 0.f , 1.1f , -0.5f };
	tTransformDesc[MINIGOLEM_HEAD].vScale = { 0.5f , 0.5f , 0.3f };

	tTransformDesc[MINIGOLEM_LEFT_ARM].vPosition = { -0.65f , 1.1f , -0.4f };
	tTransformDesc[MINIGOLEM_LEFT_ARM].vScale = { 0.3f , 0.3f , 0.8f };

	tTransformDesc[MINIGOLEM_RIGHT_ARM].vPosition = { 0.65f , 0.8f , 0.f };
	tTransformDesc[MINIGOLEM_RIGHT_ARM].vScale = { 0.3f , 1.f , 0.3f };

	tTransformDesc[MINIGOLEM_LEFT_LEG].vPosition = { -0.3f , 0.f , 0.f };
	tTransformDesc[MINIGOLEM_LEFT_LEG].vScale = { 0.3f , 1.f , 0.3f };

	tTransformDesc[MINIGOLEM_RIGHT_LEG].vPosition = { 0.3f , 0.f , 0.f };
	tTransformDesc[MINIGOLEM_RIGHT_LEG].vScale = { 0.3f , 1.f , 0.3f };

	TCHAR szCombine[MIN_STR] = L"";

	for (_uint iCnt = 0; iCnt < MINIGOLEM_END; ++iCnt)
	{
		tTransformDesc[iCnt].fSpeedPerSecond = fSpeedPerSecond;
		tTransformDesc[iCnt].fRotatePerSecond = fRotatePerSecond;

		StringCchPrintf(szCombine, _countof(szCombine), L"Com_Transform%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szCombine, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMiniGolem::Add_Component_Texture()
{
	TCHAR szComponentTag[][MIN_STR] =
	{
		L"Component_Texture_SemiBossBody",
		L"Component_Texture_SemiBossBody",
		L"Component_Texture_SemiBossHead",
		L"Component_Texture_SemiBossPart",
		L"Component_Texture_SemiBossPart",
		L"Component_Texture_SemiBossPart",
		L"Component_Texture_SemiBossPart"
	};

	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szCombine[MIN_STR] = L"";

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	for (_uint iCnt = 0; iCnt < MINIGOLEM_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, iCnt);
		if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), szComponentTag[iCnt], szCombine, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMiniGolem::Add_Component_Extends()
{
	CStatus::STAT tStat;
	tStat.iCriticalChance = 20;	tStat.iCriticalRate = 10;
	tStat.iDef = 50;
	tStat.iHp = 100;			tStat.iMp = 100;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = m_pTransformCom[MINIGOLEM_BASE]->Get_Desc().vPosition;
	tColDesc.fRadius = 0.5f;

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

HRESULT CMiniGolem::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vBasePos = m_pTransformCom[MINIGOLEM_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vBasePos))
	{
		m_pTransformCom[MINIGOLEM_BASE]->Set_Position(vBasePos);
	}

	return S_OK;
}

void CMiniGolem::Update_Disappear(_float _fDeltaTime)
{
	if (m_bActive)
	{
		m_fDisappearTime += _fDeltaTime;
		if (m_fDisappearTime >= m_fDisappearDelay)
		{
			m_fDisappearTime = 0.f;
			m_bActive = false;

			m_bCanAttack = true;
			m_fAttackTime = 0.f;
		}
	}
}

HRESULT CMiniGolem::Update_Animation_Attack(_float _fDeltaTime)
{
	if (!m_bCanAttack)
		return S_OK;

	m_fAttackTime += _fDeltaTime;
	_vec3 vMyPos = m_pTransformCom[MINIGOLEM_LEFT_ARM]->Get_Desc().vPosition;
	if (m_fAttackTime <= 1.f)
	{
		vMyPos.z -= _fDeltaTime;
		m_pTransformCom[MINIGOLEM_LEFT_ARM]->Set_Position(vMyPos);
	}
	else if (m_fAttackTime <= 2.f)
	{
		m_pTransformCom[MINIGOLEM_LEFT_ARM]->Set_Position(_vec3(vMyPos.x, vMyPos.y, -0.4f));
		Spawn_GolemImpact();
	}

	return S_OK;
}

HRESULT CMiniGolem::Spawn_GolemImpact()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(pManagement->Get_CurrentSceneID(), L"GameObject_GolemImpact", pManagement->Get_CurrentSceneID(), L"Layer_MonsterAtk")))
		return E_FAIL;

	m_bCanAttack = false;
	return S_OK;
}

void CMiniGolem::Free()
{
	for (_uint iCnt = 0; iCnt < MINIGOLEM_END; ++iCnt)
	{
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CGameObject * CMiniGolem::Clone_GameObject(void * _pArg)
{
	CMiniGolem* pInstance = new CMiniGolem(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMiniGolem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMiniGolem * CMiniGolem::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CMiniGolem* pInstance = new CMiniGolem(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMiniGolem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
