#include "stdafx.h"
#include "Player.h"
#include "..\Headers\Explosion.h"

USING(Client)

CExplosion::CExplosion(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CExplosion::CExplosion(const CExplosion & _rOther)
	:CGameObject(_rOther)
	, m_fDeadDelay(_rOther.m_fDeadDelay)
	, m_fScaleSpeed(_rOther.m_fScaleSpeed)
	, m_fScaleMin(_rOther.m_fScaleMin)
	, m_fScaleMax(_rOther.m_fScaleMax)
{ 
}

HRESULT CExplosion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CExplosion::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		m_tImpact = *(INSTANTIMPACT*)_pArg;
		m_fScaleSpeed *= m_tImpact.vOption.x;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CExplosion::Update_GameObject(_float _fDeltaTime)
{
	CPlayer* pPlayer = (CPlayer*)m_tImpact.pAttacker;

	if (m_bDead)
	{
		if (pPlayer)
			pPlayer->Buff_Off(CPlayer::BUFF_ATTACK);

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

	_float fScale = m_fScaleSpeed * 3;
	m_pTransformCom->Set_Scale(_vec3(fScale, fScale, fScale));

	m_pTransformCom->Turn(CTransform::AXIS_XYZ::AXIS_Y, _fDeltaTime * m_fScaleSpeed);

	if (FAILED(m_pTransformCom->Update_Transform(matParent)))
		return GAMEOBJECT::WARN;

	m_pTextureCom->Update_Frame(_fDeltaTime, &m_iCurrFrame);


	

	return GAMEOBJECT::NOEVENT;
}

int CExplosion::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	Update_DeadDelay(_fDeltaTime);
	Update_Scale(_fDeltaTime);
	Update_EffectDelay(_fDeltaTime);

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
		return  GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

HRESULT CExplosion::Render_OnlyAlpha()
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

HRESULT CExplosion::Render_BlendAlpha()
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

/*
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 255, 255, 255));*/

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;



	return S_OK;
}

HRESULT CExplosion::Add_Component()
{
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Explosion", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vPosition = { 0.f, -1.2f, 0.f };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { m_fScaleMin , m_fScaleMin , m_fScaleMin };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	_int iMaxCnt = m_pTextureCom->Get_TextureCount();
	m_pTextureCom->SetFrameRange(0, iMaxCnt - 1);

	m_pTransformCom->Turn(CTransform::AXIS_X, 45.f);

	return S_OK;
}


void CExplosion::Update_DeadDelay(_float _fDeltaTime)
{
	m_fDeadTimer += _fDeltaTime;

	if (m_fDeadTimer >= m_fDeadDelay)
		m_bDead = true;
}

void CExplosion::Update_EffectDelay(_float _fDeltaTime)
{
	m_fEffectTimer += _fDeltaTime;

	if (m_fEffectTimer >= m_fEffectDelay)
	{
		m_fEffectTimer = 0.f;
		Make_Pieces();
	}
}

void CExplosion::Update_Scale(_float _fDeltaTime)
{
	if (m_fScaleMax == m_pTransformCom->Get_Desc().vScale.x)
	{
		m_bScaleUp = false;
	}
	else if (m_fScaleMin == m_pTransformCom->Get_Desc().vScale.x)
	{
		m_bScaleUp = true;
	}

	//m_fScaleTimer += _fDeltaTime;
	//if (m_fScaleTimer >= 2.f)
	//{
	//	m_fScaleTimer = 0.f;
	//	m_bScaleUp = !m_bScaleUp;
	//}
}

CExplosion * CExplosion::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CExplosion* pInstance = new CExplosion(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CExplosion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CExplosion::Clone_GameObject(void * _pArg)
{
	CExplosion* pInstance = new CExplosion(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CExplosion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExplosion::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}



HRESULT CExplosion::Make_Pieces()
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	//Option에 도착을 미리 지정해서 들고오자.
	//Direction은 이동방향
	//Pos는 시작위치.


	INSTANTIMPACT pImpact;






	//--------------------------------------------------
	// TODO : 실제 메테오조각 소환
	//--------------------------------------------------

		pImpact.pAttacker = m_tImpact.pAttacker;
		pImpact.pStatusComp = nullptr;
		pImpact.vPosition = m_pTransformCom->Get_Desc().vPosition;
		pImpact.vPosition.y = 2;
		pImpact.vDirection = { 0.f, 0.f, 0.f };
		pImpact.vOption.x = m_tImpact.vOption.x;


		if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_FloatingFire", pManagement->Get_CurrentSceneID(), L"Layer_Effect", &pImpact)))
		{
			PRINT_LOG(L"Failed To Spawn FireFloatings", LOG::DEBUG);
			return false;
		}
	

	return true;
}
