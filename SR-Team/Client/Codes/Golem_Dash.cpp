#include "stdafx.h"
#include "..\Headers\Golem_Dash.h"

USING(Client)

CGolem_Dash::CGolem_Dash(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CGolem_Dash::CGolem_Dash(const CGolem_Dash& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CGolem_Dash::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CGolem_Dash::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iTexture_Number = rand() % 4;

	return S_OK;
}

_int CGolem_Dash::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Particle_Move(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	m_pTransformCom->Update_Transform();

	return GAMEOBJECT::NOEVENT;
}

_int CGolem_Dash::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime >= 1.5f)
		m_bDead = true;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CGolem_Dash * CGolem_Dash::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CGolem_Dash* pInstance = new CGolem_Dash(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CGolem_Dash", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGolem_Dash::Clone_GameObject(void * _pArg)
{
	CGolem_Dash* pInstance = new CGolem_Dash(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CGolem_Dash", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_Dash::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CGolem_Dash::Take_Damage(const CComponent * _pDamageComp)
{
	return S_OK;
}

HRESULT CGolem_Dash::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(m_iTexture_Number)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGolem_Dash::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	//시작할위치
	tTransformDesc.vPosition = { _vec3(m_tInstant.vPosition.x , m_tInstant.vPosition.y , m_tInstant.vPosition.z) };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	int iRand = rand() % 3 + 1;
	_float fScail = iRand / 10.f;

	tTransformDesc.vScale = { fScail, fScail, fScail };

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), L"Component_Texture_Bomb_Residue", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGolem_Dash::Particle_Move(_float _fDeltaTime)
{
	m_fTimeFlow += _fDeltaTime;

	if (m_fTimeFlow >= D3DX_PI)
		m_fTimeFlow = 0;

	_vec3 Temp = m_pTransformCom->Get_Desc().vPosition;

	Temp.y = sinf(m_fTimeFlow);

	m_pTransformCom->Set_Position(Temp);

	return S_OK;
}
