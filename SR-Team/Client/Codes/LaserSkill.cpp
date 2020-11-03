#include "stdafx.h"
#include "..\Headers\LaserSkill.h"
USING(Client)


CLaserSkill::CLaserSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerSkill(_pDevice, _pSprite, _pFont)
{
}

CLaserSkill::CLaserSkill(const CLaserSkill & _rOther)
	: CPlayerSkill(_rOther)
{
}

HRESULT CLaserSkill::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CLaserSkill::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 5.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CLaserSkill::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CLaserSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CLaserSkill * CLaserSkill::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CLaserSkill* pInstance = new CLaserSkill(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CLaserSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLaserSkill::Clone_GameObject(void * _pArg)
{
	CLaserSkill* pInstance = new CLaserSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CLaserSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CLaserSkill::Actual_UseSkill(void* _pArg)
{
	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	//--------------------------------------------------
	// TODO : 레이저 소환
	//--------------------------------------------------


	--m_iCanUseCnt;
	return true;
}
