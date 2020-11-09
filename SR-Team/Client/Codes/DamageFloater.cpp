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

	for (_uint iCnt = 0; iCnt < 4; ++iCnt)
	{
//		m_tInfo.iDamage % ;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CDamageFloat::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	return GAMEOBJECT::NOEVENT;
}

_int CDamageFloat::LateUpdate_GameObject(_float _fDeltaTime)
{


	return GAMEOBJECT::NOEVENT;
}

HRESULT CDamageFloat::Render_NoneAlpha()
{
	return S_OK;
}

void CDamageFloat::Set_FloatValue(_int _iFloatValue)
{
}

HRESULT CDamageFloat::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = m_tInfo.vSpawnPos;
	tTransformDesc.vScale = { 1.f, 1.f, 1.f };
	tTransformDesc.vRotate = { D3DXToRadian(45.f), D3DXToRadian(-90.f), 0.f };
	tTransformDesc.fSpeedPerSecond = 4.5f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(160.f);



	return S_OK;
}

CDamageFloat * CDamageFloat::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	return nullptr;
}

CGameObject * CDamageFloat::Clone_GameObject(void * _pArg)
{
	return nullptr;
}

void CDamageFloat::Free()
{
}
