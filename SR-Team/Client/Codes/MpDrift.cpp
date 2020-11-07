#include "stdafx.h"
#include "Player.h"
#include "..\Headers\MpDrift.h"


USING(Client)

CMpDrift::CMpDrift(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CMpDrift::CMpDrift(const CMpDrift& other)
	: CGameObject(other)
	,m_fDeadDelay(other.m_fDeadDelay)
{
}

HRESULT CMpDrift::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMpDrift::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tImpact = *(INSTANTIMPACT*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CMpDrift::Update_GameObject(float _fDeltaTime)
{
	CPlayer* pPlayer = (CPlayer*)m_tImpact.pAttacker;

	if (m_bDead)
	{
		if (pPlayer)
			pPlayer->Buff_Off(CPlayer::BUFF_MANA);

		return GAMEOBJECT::DEAD;
	}

	_matrix matParent;
	D3DXMatrixIdentity(&matParent);

	if (pPlayer)
	{
		CTransform* pTransform = (CTransform*)pPlayer->Get_Component(L"Com_Transform1");
		if (pTransform)
			matParent = pTransform->Get_Desc().matWorld;
	}

	if (FAILED(m_pTransformCom->Update_Transform(matParent)))
		return GAMEOBJECT::WARN;

	m_pTextureCom->Update_Frame(_fDeltaTime, &m_iCurrFrame);

	return GAMEOBJECT::NOEVENT;
}

int CMpDrift::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	Update_DeadDelay(_fDeltaTime);

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_ONLYALPHA, this)))
		return  GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

HRESULT CMpDrift::Render_OnlyAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(m_iCurrFrame)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMpDrift::Add_Component()
{
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_MPDrift", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vPosition = { 0.f, -1.0f, 0.f };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 2.2f , 2.2f , 2.2f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	_int iMaxCnt = m_pTextureCom->Get_TextureCount();
	m_pTextureCom->SetFrameRange(0, iMaxCnt - 1);

	m_pTransformCom->Turn(CTransform::AXIS_X, 45.f);
	return S_OK;
}

void CMpDrift::Update_DeadDelay(float _fDeltaTime)
{
	m_fDeadTimer += _fDeltaTime;

	if (m_fDeadTimer >= m_fDeadDelay)
		m_bDead = true;
}

CMpDrift * CMpDrift::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMpDrift* pInstance = new CMpDrift(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMpDrift", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMpDrift::Clone_GameObject(void * pArg)
{
	CMpDrift* pInstance = new CMpDrift(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMpDrift", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMpDrift::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
