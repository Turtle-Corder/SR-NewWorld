#include "stdafx.h"
#include "..\Headers\DamageFloater.h"

USING(Client)

CDamageFloat::CDamageFloat(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < 4; ++iCnt)
	{
		m_pVIBufferCom[iCnt] = nullptr;
		m_pTransformCom[iCnt] = nullptr;
	}
}

CDamageFloat::CDamageFloat(CONST CDamageFloat & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CDamageFloat::Setup_GameObject_Prototype()
{	
	return S_OK;
}

HRESULT CDamageFloat::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_tInfo = *(FLOATING_INFO*)_pArg;

	_int iDamage = m_tInfo.iDamage;

	// 최소 한자리는 있겠지..
	m_iDigitNum[0] = iDamage % 10;
	iDamage /= 10;

	vAddRandomDir = { (_float)(rand() % 10) , (_float)(rand() % 10) ,(_float)(rand() % 10) };

	for (_int iCnt = 1; iCnt < 4; ++iCnt)
	{ 
		m_iDigitNum[iCnt] = iDamage % 10;
		if (0 != m_iDigitNum[iCnt])
			m_iMaxDigit = iCnt + 1;

		iDamage /= 10;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CDamageFloat::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	vAddRandomDir.y -= 9.8f * _fDeltaTime;


	for (_int iCnt = 0; iCnt < m_iMaxDigit; ++iCnt)
	{
		_vec3 vAddPos = _vec3(0.f, 1.f, 0.f) * m_pTransformCom[iCnt]->Get_Desc().fSpeedPerSecond * _fDeltaTime;
		m_pTransformCom[iCnt]->Set_Position(m_pTransformCom[iCnt]->Get_Desc().vPosition + vAddPos);
		m_pTransformCom[iCnt]->Update_Transform();
	}

	if (FAILED(IsBillboarding()))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CDamageFloat::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	Update_DeadDelay(_fDeltaTime);

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_ONLYALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CDamageFloat::Render_OnlyAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int iCnt = 0; iCnt < m_iMaxDigit; ++iCnt)
	{
		if (FAILED(m_pVIBufferCom[iCnt]->Set_Transform(&m_pTransformCom[iCnt]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom->SetTexture(m_iDigitNum[iCnt])))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iCnt]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDamageFloat::Add_Component()
{
	TCHAR szName[MIN_STR] = L"";

	//--------------------------------------------------
	// Transform
	//--------------------------------------------------
	CTransform::TRANSFORM_DESC tTransformDesc[4];
	//Camera Vector 획득
	CManagement* pManagement = CManagement::Get_Instance();
	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	_vec3 vTermTest = pCamera->Get_Desc().vEye - pCamera->Get_Desc().vAt;
	
	D3DXVec3Normalize(&vTermTest, &vTermTest);

	vTermTest.y = 0;
	_vec3 vInitSacle = { 1.f, 1.f, 1.f };	
	_vec3 vTerm = { vTermTest.z, 0.f, -vTermTest.x };
	//

	for (_int iCnt = 0; iCnt < m_iMaxDigit; ++iCnt)
	{
		tTransformDesc[iCnt].vPosition = m_tInfo.vSpawnPos + vTerm * (_float)iCnt * 1.3f;
		tTransformDesc[iCnt].vScale = vInitSacle;
		tTransformDesc[iCnt].fSpeedPerSecond = 5.f;

		StringCchPrintf(szName, _countof(szName), L"Com_Transform%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt])))
			return E_FAIL;
	}

	//--------------------------------------------------
	// VIBuffer
	//--------------------------------------------------
	for (_int iCnt = 0; iCnt < m_iMaxDigit; ++iCnt)
	{
		StringCchPrintf(szName, _countof(szName), L"Com_VIBuffer%d", iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", szName, (CComponent**)&m_pVIBufferCom[iCnt])))
			return E_FAIL;

	}

	//--------------------------------------------------
	// Texture
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Textrue_Damage", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamageFloat::IsBillboarding()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	const D3DXMATRIX* pView = pCamera->Get_ViewMatrix();
	if (nullptr == pView)
		return E_FAIL;

	//D3DXMATRIX matBillY;
	//D3DXMatrixIdentity(&matBillY);

	//// Y축 빌보드 (ex: 풀, 나무 등)
	//// 뷰행렬의 Y축 회전정보를 얻어옴.
	//matBillY._11 = pView->_11;
	//matBillY._13 = pView->_13;
	//matBillY._31 = pView->_31;
	//matBillY._33 = pView->_33;

	//D3DXMatrixInverse(&matBillY, nullptr, &matBillY);

	//for (_int iCnt = 0; iCnt < m_iMaxDigit; ++iCnt)
	//{
	//	D3DXMATRIX matWorld = m_pTransformCom[iCnt]->Get_Desc().matWorld;
	//	m_pTransformCom[iCnt]->Set_WorldMatrix(matBillY * matWorld);
	//}

	D3DXMATRIX matBill;
	D3DXMatrixIdentity(&matBill);

	// 뷰행렬의 모든 회전정보를 얻어옴.
	memcpy(&matBill.m[0][0], &pView->m[0][0], sizeof(D3DXVECTOR3));
	memcpy(&matBill.m[1][0], &pView->m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&matBill.m[2][0], &pView->m[2][0], sizeof(D3DXVECTOR3));
	
	D3DXMatrixInverse(&matBill, nullptr, &matBill);

	for (_int iCnt = 0; iCnt < m_iMaxDigit; ++iCnt)
	{
		D3DXMATRIX matWorld = m_pTransformCom[iCnt]->Get_Desc().matWorld;
		m_pTransformCom[iCnt]->Set_WorldMatrix(matBill * matWorld);
	}

	return S_OK;
}

void CDamageFloat::Update_DeadDelay(_float _fDeltaTime)
{
	m_fDeadTimer += _fDeltaTime;
	if (m_fDeadTimer >= m_fDeadDelay)
		m_bDead = true;
}

CDamageFloat * CDamageFloat::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CDamageFloat* pInstance = new CDamageFloat(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CDamageFloat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDamageFloat::Clone_GameObject(void * _pArg)
{
	CDamageFloat* pInstance = new CDamageFloat(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CDamageFloat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageFloat::Free()
{
	for (_int iCnt = 0; iCnt < m_iMaxDigit; ++iCnt)
	{
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTransformCom[iCnt]);
	}

	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
