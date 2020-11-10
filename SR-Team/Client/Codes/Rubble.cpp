#include "stdafx.h"
#include "..\Headers\Rubble.h"

USING(Client)

CRubble::CRubble(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CRubble::CRubble(const CRubble & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CRubble::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRubble::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fJumpPower = (_float)(rand() % 8 + 2);

	return S_OK;
}

_int CRubble::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Particle_Move(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	m_pTransformCom->Update_Transform();


	return GAMEOBJECT::NOEVENT;
}

_int CRubble::LateUpdate_GameObject(_float _fDeltaTime)
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

CRubble * CRubble::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CRubble* pInstance = new CRubble(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CRubble", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRubble::Clone_GameObject(void * _pArg)
{
	CRubble* pInstance = new CRubble(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CRubble", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRubble::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CRubble::Take_Damage(const CComponent * _pDamageComp)
{
	return S_OK;
}

HRESULT CRubble::Render_NoneAlpha()
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

HRESULT CRubble::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	//시작할위치
	tTransformDesc.vPosition = { _vec3(m_tInstant.vPosition.x , m_tInstant.vPosition.y , m_tInstant.vPosition.z) };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 0.3f, 0.3f, 0.3f };

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

HRESULT CRubble::Particle_Move(_float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

	vPos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f;
	m_fJumpTime += _fDeltaTime;

	if (vPos.y < m_pTransformCom->Get_Desc().vPosition.y)
	{
		m_bDead = true;
	}

	m_pTransformCom->Set_Position(vPos);

	return S_OK;
}
