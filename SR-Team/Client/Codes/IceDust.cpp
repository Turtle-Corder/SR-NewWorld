#include "stdafx.h"
#include "..\Headers\IceDust.h"
#include "Player.h"

USING(Client)

CIceDust::CIceDust(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CIceDust::CIceDust(const CIceDust & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CIceDust::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CIceDust::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_vInitPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iRandRot = rand() % 3;

	m_fLifeTimer = 0.f;
	m_fLife = 2.f;
	return S_OK;
}

_int CIceDust::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	switch (m_iRandRot)
	{
	case 0:
		m_pTransformComp->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformComp->Turn(CTransform::AXIS_Y, _fDeltaTime);
		break;

	case 1:
		m_pTransformComp->Turn(CTransform::AXIS_Y, _fDeltaTime);
		m_pTransformComp->Turn(CTransform::AXIS_Z, _fDeltaTime);
		break;

	case 2:
		m_pTransformComp->Turn(CTransform::AXIS_X, _fDeltaTime);
		m_pTransformComp->Turn(CTransform::AXIS_Z, _fDeltaTime);
		break;

	default:
		break;
	}

	_vec3 vPos = m_pTransformComp->Get_Desc().vPosition;
	vPos.y -= _fDeltaTime;
	m_pTransformComp->Set_Position(vPos);

	if (FAILED(m_pTransformComp->Update_Transform()))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

_int CIceDust::LateUpdate_GameObject(_float _fDeltaTime)
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;



	m_fLifeTimer += _fDeltaTime;
	if (m_fLife < m_fLifeTimer)
		m_bDead = true;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CIceDust::Render_NoneAlpha()
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

HRESULT CIceDust::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = m_vInitPos;
	tTransformDesc.vScale = { 0.05f, 0.07f, 0.05f };
	tTransformDesc.vRotate = { D3DXToRadian(45.f), 0.f, D3DXToRadian(45.f) };
	tTransformDesc.fSpeedPerSecond = 4.5f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferComp)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_IceBlast", L"Com_Texture", (CComponent**)&m_pTextureComp)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformComp, &tTransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CIceDust::Floating(_float _fDeltaTime)
{
	m_fFloatTime += _fDeltaTime * 2.f;
	if (m_fFloatTime >= D3DX_PI)
		m_fFloatTime = 0.f;

	_vec3 vPos = { m_pTransformComp->Get_Desc().vPosition.x, sinf(m_fFloatTime) * 0.09f, m_pTransformComp->Get_Desc().vPosition.z };

	m_pTransformComp->Set_Position(vPos);

	return S_OK;
}


CGameObject * CIceDust::Clone_GameObject(void * _pArg)
{
	CIceDust* pInstance = new CIceDust(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create EnergyBolt Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceDust::Free()
{
	Safe_Release(m_pVIBufferComp);
	Safe_Release(m_pTransformComp);
	Safe_Release(m_pTextureComp);

	CGameObject::Free();
}

CIceDust * CIceDust::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CIceDust* pInstance = new CIceDust(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CIceCrystal", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
