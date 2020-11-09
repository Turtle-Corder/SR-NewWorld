#include "stdafx.h"
#include "..\Headers\MeteorPiece.h"

USING(Client)

CMeteorPiece::CMeteorPiece(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CMeteorPiece::CMeteorPiece(const CMeteorPiece& other)
	: CGameObject(other)
{
}

HRESULT CMeteorPiece::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMeteorPiece::Setup_GameObject(void* _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
		//Option에 도착방향을 미리 지정해서 들고오자.
		//Direction은 이동방향
		//Pos는 시작위치.
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vMoveDir = m_tInstant.vDirection;
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

	return S_OK;
}

int CMeteorPiece::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

int CMeteorPiece::LateUpdate_GameObject(_float _fDeltaTime)
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

HRESULT CMeteorPiece::Render_NoneAlpha()
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

HRESULT CMeteorPiece::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	//시작할위치
	tTransformDesc.vPosition = { _vec3(m_tInstant.vPosition.x + 0.05f , m_tInstant.vPosition.y , m_tInstant.vPosition.z - 0.05f) };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(135.f);
	tTransformDesc.vScale = { 1.2f, 2.3f, 1.2f };

	CSphereCollider::COLLIDER_DESC tCollDesc;
	tCollDesc.vPosition = tTransformDesc.vPosition;
	tCollDesc.fRadius = 0.5f;

	CStatus::STAT tStat;
	tStat.iMinAtt = 10; tStat.iMaxAtt = 10;

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

HRESULT CMeteorPiece::Movement(_float _fDeltaTime)
{
	if (FAILED(ParticleMove(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeteorPiece::ParticleMove(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vAddPos = m_vMoveDir * m_pTransformCom->Get_Desc().fSpeedPerSecond * _fDeltaTime;
	m_pTransformCom->Set_Position(m_pTransformCom->Get_Desc().vPosition + vAddPos);

	m_pTransformCom->Turn(CTransform::AXIS_XYZ::AXIS_X, _fDeltaTime * 0.5f);
	m_pTransformCom->Turn(CTransform::AXIS_XYZ::AXIS_Y, _fDeltaTime *  0.5f);
	m_pTransformCom->Turn(CTransform::AXIS_XYZ::AXIS_Z, _fDeltaTime *  0.5f);


	_float Length = D3DXVec3Length(&(m_pTransformCom->Get_Desc().vPosition - m_tInstant.vOption));
	if (Length < 0.5f)
		m_bDead = true;

	return S_OK;
}

CMeteorPiece * CMeteorPiece::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMeteorPiece* pInstance = new CMeteorPiece(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMeteor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeteorPiece::Clone_GameObject(void * pArg)
{
	CMeteorPiece* pInstance = new CMeteorPiece(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMeteor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeteorPiece::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CMeteorPiece::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;



	return S_OK;
}
