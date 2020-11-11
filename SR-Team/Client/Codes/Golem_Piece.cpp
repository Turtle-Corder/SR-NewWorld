#include "stdafx.h"
#include "..\Headers\Golem_Piece.h"

USING(Client)

CGolem_Piece::CGolem_Piece(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CGolem_Piece::CGolem_Piece(const CGolem_Piece & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CGolem_Piece::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CGolem_Piece::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;


	return S_OK;
}

_int CGolem_Piece::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Particle_Move(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	m_pTransformCom->Update_Transform();


	return GAMEOBJECT::NOEVENT;
}

_int CGolem_Piece::LateUpdate_GameObject(_float _fDeltaTime)
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

CGolem_Piece * CGolem_Piece::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CGolem_Piece* pInstance = new CGolem_Piece(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CGolem_Piece", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGolem_Piece::Clone_GameObject(void * _pArg)
{
	CGolem_Piece* pInstance = new CGolem_Piece(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CGolem_Piece", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_Piece::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CGolem_Piece::Take_Damage(const CComponent * _pDamageComp)
{
	return S_OK;
}

HRESULT CGolem_Piece::Render_NoneAlpha()
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

HRESULT CGolem_Piece::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	//시작할위치
	tTransformDesc.vPosition = { _vec3(m_tInstant.vPosition.x , m_tInstant.vPosition.y , m_tInstant.vPosition.z) };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 0.5f, 0.5f, 0.5f };

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Meteor", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGolem_Piece::Particle_Move(_float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

	vPos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f;
	m_fJumpTime += _fDeltaTime;

	if (vPos.y < 0.f)
	{
		m_bDead = true;
	}

	m_pTransformCom->Set_Position(vPos);

	return S_OK;
}
