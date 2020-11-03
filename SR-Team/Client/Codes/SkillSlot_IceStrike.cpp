#include "stdafx.h"
#include "..\Headers\SkillSlot_IceStrike.h"

USING(Client)



CSkillSlot_IceStrike::CSkillSlot_IceStrike(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_IceStrike::CSkillSlot_IceStrike(const CSkillSlot_IceStrike & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_IceStrike::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_IceStrike::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 5.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_IceStrike::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_IceStrike::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_IceStrike * CSkillSlot_IceStrike::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkillSlot_IceStrike* pInstance = new CSkillSlot_IceStrike(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CIceStrikeSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_IceStrike::Clone_GameObject(void * _pArg)
{
	CSkillSlot_IceStrike* pInstance = new CSkillSlot_IceStrike(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CIceStrikeSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_IceStrike::Actual_UseSkill(void * _pArg)
{
	if (!Can_UseSkill())
		return false;

	// TODO : ¼ÒÈ¯..
	if (_pArg)
	{
		INSTANTIMPACT* pImpact = (INSTANTIMPACT*)_pArg;
		CStatus* pStatus = (CStatus*)pImpact->pStatusComp;
		if (pStatus)
			pStatus->Increase_IceStack();
	}


	--m_iCanUseCnt;
	return true;
}
