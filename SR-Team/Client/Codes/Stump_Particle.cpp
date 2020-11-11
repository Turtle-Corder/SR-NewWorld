#include "stdafx.h"
#include "..\Headers\Stump_Particle.h"

USING(Client)

CStump_Particle::CStump_Particle(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CStump_Particle::CStump_Particle(const CStump_Particle& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CStump_Particle::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CStump_Particle::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_matStumpWorld = *(_matrix*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iTexture_Number = rand() % 4;

	return S_OK;
}

_int CStump_Particle::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Particle_Move(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	m_pTransformCom->Update_Transform();

	if (FAILED(Set_WorldMatrix(_fDeltaTime)))
		return E_FAIL;

	return GAMEOBJECT::NOEVENT;
}

_int CStump_Particle::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime >= 3.f)
		m_bDead = true;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CStump_Particle * CStump_Particle::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CStump_Particle* pInstance = new CStump_Particle(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CStump_Particle", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStump_Particle::Clone_GameObject(void * _pArg)
{
	CStump_Particle* pInstance = new CStump_Particle(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CStump_Particle", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStump_Particle::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CStump_Particle::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
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

HRESULT CStump_Particle::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	_float fX = (_float)(rand() % 6 - 3); _float fZ = (_float)(rand() % 6 - 3); _float fY = (_float)(rand() % 10) + 1.f;
	_float fMyY = (fY / 10.f) * 2.f;

	if (fX == 0)	
		fX = 2.f;
	if(fZ == 0)
		fZ = 2.f;

	if (fX < 0)
		fX += -2.f;
	else if (fX > 0)
		fX += 2.f;

	if (fZ < 0)
		fZ += -2.f;
	else if (fZ > 0)
		fZ += 2.f;

	tTransformDesc.vPosition = { fX , fMyY , fZ };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	int iRand = rand() % 5 + 2;
	_float fScail = iRand / 10.f;

	tTransformDesc.vScale = { fScail, fScail, fScail };

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), L"Component_Texture_Acorn", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CStump_Particle::Particle_Move(_float _fDeltaTime)
{
	m_fTimeFlow += _fDeltaTime;

	if (m_fTimeFlow >= D3DX_PI)
		m_fTimeFlow = 0;

	_vec3 Temp = m_pTransformCom->Get_Desc().vPosition;

	Temp.y = sinf(m_fTimeFlow);

	m_pTransformCom->Set_Position(Temp);

	m_pTransformCom->Turn(CTransform::AXIS_X, _fDeltaTime * 3.f);
	m_pTransformCom->Turn(CTransform::AXIS_Z, _fDeltaTime * 3.f);
	return S_OK;
}

HRESULT CStump_Particle::Set_WorldMatrix(_float _fDeltaTime)
{
	m_fAngle += 4.f;

	if (m_fAngle >= 360.f)
		m_fAngle = 0.f;

	_matrix RevY;
	_matrix vMyMat = m_pTransformCom->Get_Desc().matWorld;
	_matrix vPar = m_matStumpWorld;
	D3DXMatrixRotationY(&RevY, D3DXToRadian(m_fAngle));

	m_pTransformCom->Set_WorldMatrix(vMyMat * RevY * vPar);

	return S_OK;
}
