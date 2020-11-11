#include "stdafx.h"
#include "Player.h"
#include "..\Headers\FloatingFire.h"


USING(Client)

CFloatingFire::CFloatingFire(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CFloatingFire::CFloatingFire(const CFloatingFire & _rOther)
	: CGameObject(_rOther)
	, m_fDeadDelay(_rOther.m_fDeadDelay)
{
}

HRESULT CFloatingFire::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CFloatingFire::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tImpact = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;



	m_fAngle = ((_float)rand() / (_float)RAND_MAX) * D3DX_PI * 2 ;

	m_fCircleRange = m_tImpact.vOption.x;

	m_pTransformComp->Turn(CTransform::AXIS_XYZ::AXIS_Y, m_fAngle);


	return S_OK;
}

int CFloatingFire::Update_GameObject(_float _fDeltaTime)
{
	CPlayer* pPlayer = (CPlayer*)m_tImpact.pAttacker;

	if (m_bDead)
	{
		if (pPlayer)
			pPlayer->Buff_Off(CPlayer::BUFF_ATTACK);

		return GAMEOBJECT::DEAD;
	}

	if (FAILED(m_pTransformComp->Update_Transform()))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

int CFloatingFire::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	Update_DeadDelay(_fDeltaTime);


	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
		return  GAMEOBJECT::WARN;

	Change_Position(_fDeltaTime);

	return GAMEOBJECT::NOEVENT;
}

HRESULT CFloatingFire::Render_BlendAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

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

HRESULT CFloatingFire::Add_Component()
{
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferComp)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_FloatingFire", L"Com_Texture", (CComponent**)&m_pTextureComp)))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vPosition = { 0.f, 0.f, 0.f };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = 1.f;
	tTransformDesc.vScale = { 0.5f , 0.5f , 0.5f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformComp, &tTransformDesc)))
		return E_FAIL;



	return S_OK;
}


void CFloatingFire::Update_DeadDelay(_float _fDeltaTime)
{
	m_fDeadTimer += _fDeltaTime;

	if (m_fDeadTimer >= m_fDeadDelay)
		m_bDead = true;
}

void CFloatingFire::Change_Position(_float _fDeltaTime)
{

	CPlayer* pPlayer = (CPlayer*)m_tImpact.pAttacker;

	_vec3 vPlayerPos;
	_vec3 vScaleVector;
	if (pPlayer)
	{
		CTransform* pTransform = (CTransform*)pPlayer->Get_Component(L"Com_Transform1");
		if (pTransform)
			vPlayerPos = pTransform->Get_Desc().vPosition;
	}

	vScaleVector = m_pTransformComp->Get_Desc().vScale;

	m_fYPosChanger -= 5.f * _fDeltaTime;

	vScaleVector.y += m_fYPosChanger * _fDeltaTime;
	vScaleVector.x = vScaleVector.z *= 0.9f;

	m_pTransformComp->Set_Scale(vScaleVector);

	if (m_fYPosChanger < -20.f)
		m_bDead = true;

	// falldown
	_vec3 vAddPos = _vec3(sin(m_fAngle), m_fYPosChanger/9.8f, cos(m_fAngle)) * (m_fCircleRange/m_tImpact.vOption.x);
	m_pTransformComp->Set_Position(vPlayerPos + vAddPos * m_fCircleRange * 0.7f);

}

CFloatingFire * CFloatingFire::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CFloatingFire* pInstance = new CFloatingFire(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CExplosion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFloatingFire::Clone_GameObject(void * _pArg)
{
	CFloatingFire* pInstance = new CFloatingFire(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CExplosion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFloatingFire::Free()
{
	Safe_Release(m_pTransformComp);
	Safe_Release(m_pVIBufferComp);
	Safe_Release(m_pTextureComp);

	CGameObject::Free();
}
