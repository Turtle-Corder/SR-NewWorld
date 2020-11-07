#include "stdafx.h"
#include "Player.h"
#include "..\Headers\IceCrystal.h"

USING(Client)

CIceCrystal::CIceCrystal(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CIceCrystal::CIceCrystal(const CIceCrystal & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CIceCrystal::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CIceCrystal::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_vInitPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iRandRot = rand() % 3;

	return S_OK;
}

_int CIceCrystal::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player");
	if (nullptr == pPlayer)
		return GAMEOBJECT::NOEVENT;

	CTransform* pPlayerTransform = (CTransform*)pPlayer->Get_Component(L"Com_Transform0");
	if (nullptr == pPlayerTransform)
		return GAMEOBJECT::NOEVENT;
	
	Floating(_fDeltaTime);

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

	if (FAILED(m_pTransformComp->Update_Transform(pPlayerTransform->Get_Desc().matWorld)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CIceCrystal::LateUpdate_GameObject(_float _fDeltaTime)
{
	// renderer list에 등록 안함
	if (!m_bActive)
		GAMEOBJECT::NOEVENT;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CIceCrystal::Render_NoneAlpha()
{
	if (!m_bActive)
		return S_OK;

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

HRESULT CIceCrystal::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = m_vInitPos;
	tTransformDesc.vScale = { 0.33f, 0.33f, 0.33f };
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

HRESULT CIceCrystal::Floating(_float _fDeltaTime)
{
	m_fFloatTime += _fDeltaTime * 2.f;
	if (m_fFloatTime >= D3DX_PI)
		m_fFloatTime = 0.f;

	_vec3 vPos = { m_pTransformComp->Get_Desc().vPosition.x, sinf(m_fFloatTime) * 0.5f, m_pTransformComp->Get_Desc().vPosition.z };

	m_pTransformComp->Set_Position(vPos);

	return S_OK;
}


CGameObject * CIceCrystal::Clone_GameObject(void * _pArg)
{
	CIceCrystal* pInstance = new CIceCrystal(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create EnergyBolt Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceCrystal::Free()
{
	Safe_Release(m_pVIBufferComp);
	Safe_Release(m_pTransformComp);
	Safe_Release(m_pTextureComp);

	CGameObject::Free();
}

CIceCrystal * CIceCrystal::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CIceCrystal* pInstance = new CIceCrystal(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CIceCrystal", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
