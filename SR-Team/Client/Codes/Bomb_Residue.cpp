#include "stdafx.h"
#include "..\Headers\Bomb_Residue.h"

USING(Client)

CBomb_Residue::CBomb_Residue(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CBomb_Residue::CBomb_Residue(const CBomb_Residue& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CBomb_Residue::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBomb_Residue::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInstant = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iRandSpeed = rand() % 4 + 1;
	m_vMoveDir = m_tInstant.vDirection;

	return S_OK;
}

_int CBomb_Residue::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Particle_Move(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	m_pTransformCom->Update_Transform();


	return GAMEOBJECT::NOEVENT;
}

_int CBomb_Residue::LateUpdate_GameObject(_float _fDeltaTime)
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

CBomb_Residue* CBomb_Residue::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CBomb_Residue* pInstance = new CBomb_Residue(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CBomb_Residue", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBomb_Residue::Clone_GameObject(void * _pArg)
{
	CBomb_Residue* pInstance = new CBomb_Residue(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CBomb_Residue", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBomb_Residue::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CBomb_Residue::Take_Damage(const CComponent * _pDamageComp)
{
	return S_OK;
}

HRESULT CBomb_Residue::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBomb_Residue::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	//시작할위치
	tTransformDesc.vPosition = { _vec3(m_tInstant.vPosition.x , m_tInstant.vPosition.y , m_tInstant.vPosition.z) };
	tTransformDesc.fSpeedPerSecond = 1.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(120.f);

	_uint iRand = (rand() % 4 + 1) + 2;
	_float fScail = iRand / 10.f;
	tTransformDesc.vScale = { fScail, fScail, fScail };

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(pManagement->Get_CurrentSceneID(), L"Component_Texture_Bomb", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBomb_Residue::Particle_Move(_float _fDeltaTime)
{
	/*_vec3 vMypos = m_pTransformCom->Get_Desc().vPosition;

	vMypos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f;

	m_fJumpTime += _fDeltaTime;

	if (vMypos.y < m_pTransformCom->Get_Desc().vPosition.y)
	{
		m_bDead = true;
	}

	vMypos += m_tInstant.vDirection * (_fDeltaTime * m_iRandSpeed);

	m_pTransformCom->Set_Position(vMypos);
	*/
	
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vAddPos = m_vMoveDir * m_pTransformCom->Get_Desc().fSpeedPerSecond * _fDeltaTime;
	m_vMoveDir.y -= _fDeltaTime * 9.8f;
	m_pTransformCom->Set_Position(m_pTransformCom->Get_Desc().vPosition + vAddPos);

	m_pTransformCom->Turn(CTransform::AXIS_XYZ::AXIS_X, _fDeltaTime * 0.5f);
	m_pTransformCom->Turn(CTransform::AXIS_XYZ::AXIS_Y, _fDeltaTime *  0.5f);
	m_pTransformCom->Turn(CTransform::AXIS_XYZ::AXIS_Z, _fDeltaTime *  0.5f);


	_float Length = D3DXVec3Length(&(m_pTransformCom->Get_Desc().vPosition - m_tInstant.vOption));
	if (Length < 0.5f)
		m_bDead = true;

	return S_OK;
}
