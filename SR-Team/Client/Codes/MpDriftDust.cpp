#include "stdafx.h"
#include "Player.h"
#include "..\Headers\MpDriftDust.h"


USING(Client)

CMpDriftDust::CMpDriftDust(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CMpDriftDust::CMpDriftDust(const CMpDriftDust & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CMpDriftDust::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMpDriftDust::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tImpact = *(INSTANTIMPACT*)_pArg;
		m_fInitDelay  = m_tImpact.vOption.x *0.01f;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fAngle = (_float)(rand() % 314)* 0.01f;
	m_fCircleRange = (_float)(rand() % 7 + 15)* 0.1f;
	return S_OK;
}

_int CMpDriftDust::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;


	CPlayer* pPlayer = (CPlayer*)m_tImpact.pAttacker;

	_vec3 vPlayerPos;

	if (pPlayer)
	{
		CTransform* pTransform = (CTransform*)pPlayer->Get_Component(L"Com_Transform1");
		if (pTransform)
			vPlayerPos = pTransform->Get_Desc().vPosition;
	}




	m_fAngle += _fDeltaTime * 10.f;
	// falldown
	_vec3 vAddPos = _vec3(sin(m_fAngle), cos(m_fAngle * 1.7f), cos(m_fAngle)) * m_fCircleRange;
	m_pTransformComp->Set_Position(vPlayerPos + vAddPos * 0.7f);




	m_pTransformComp->Update_Transform();

	return GAMEOBJECT::NOEVENT;
}

_int CMpDriftDust::LateUpdate_GameObject(_float _fDeltaTime)
{
	// init delay counting
	Update_InitDelay(_fDeltaTime);
	Update_DeadDelay(_fDeltaTime);

	if (!m_bActive)
		GAMEOBJECT::NOEVENT;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMpDriftDust::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");

	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferComp->Set_Transform(&m_pTransformComp->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureComp->SetTexture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferComp->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMpDriftDust::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vScale = { 0.1f, 0.1f, 0.1f };
	tTransformDesc.vRotate = { D3DXToRadian(_float(rand() % 90)), D3DXToRadian(_float(rand() % 90)), D3DXToRadian(_float(rand() % 90)) };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferComp)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_IceBlast", L"Com_Texture", (CComponent**)&m_pTextureComp)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformComp, &tTransformDesc)))
		return E_FAIL;



	return S_OK;
}

void CMpDriftDust::Update_InitDelay(_float _fDeltaTime)
{
	if (!IsActive())
	{
		m_fInitTimer += _fDeltaTime;
		if (m_fInitTimer >= m_fInitDelay)
			Set_Active();
	}
}

void CMpDriftDust::Update_DeadDelay(_float _fDeltaTime)
{
	if (IsActive() && !m_bDead)
	{
		m_fDeadTimer += _fDeltaTime;
		if (m_fDeadTimer >= m_fDeadDelay)
			m_bDead = true;
	}
}




CGameObject * CMpDriftDust::Clone_GameObject(void * _pArg)
{
	CMpDriftDust* pInstance = new CMpDriftDust(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create EnergyBolt Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMpDriftDust::Free()
{
	Safe_Release(m_pVIBufferComp);
	Safe_Release(m_pTransformComp);
	Safe_Release(m_pTextureComp);

	CGameObject::Free();
}

CMpDriftDust * CMpDriftDust::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CMpDriftDust* pInstance = new CMpDriftDust(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CIceBlast", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

