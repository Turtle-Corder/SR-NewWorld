#include "stdafx.h"
#include "Player.h"
#include "..\Headers\Blind.h"

USING(Client)

CBlind::CBlind(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CBlind::CBlind(const CBlind & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CBlind::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBlind::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tImpact = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBlind::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (m_tImpact.pTarget)
	{
		CTransform* pTargetTransform = (CTransform*)m_tImpact.pTarget->Get_Component(L"Com_Transform0");
		if (pTargetTransform)
			m_pTransformCom->Update_Transform(pTargetTransform->Get_Desc().matWorld);
	}

	return GAMEOBJECT::NOEVENT;
}

_int CBlind::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	Update_DeadDelay(_fDeltaTime);
	Update_TargetDeadCheck();

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;

}

CGameObject * CBlind::Clone_GameObject(void * _pArg)
{
	CBlind* pInstance = new CBlind(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create Clone CShield", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CBlind::Render_BlendAlpha()
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

CBlind * CBlind::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CBlind* pInstance = new CBlind(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CBlind", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlind::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CBlind::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORM_DESC));

	tTransformDesc.vPosition = { 0.f , 0.f , 0.f };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Blind", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


void CBlind::Update_DeadDelay(_float _fDeltaTime)
{
	m_fDeadTime += _fDeltaTime;
	if (m_fDeadTime > 4.f)
		m_bDead = true;
}

void CBlind::Update_TargetDeadCheck()
{
	if (m_tImpact.pTarget)
	{
		if (m_tImpact.pTarget->IsDead())
			m_bDead = true;
	}
}
