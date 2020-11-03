#include "stdafx.h"
#include "..\Headers\SkillSlot_EnergyExlposion.h"


USING(Client)

CSkillSlot_EnergyExplosion::CSkillSlot_EnergyExplosion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_EnergyExplosion::CSkillSlot_EnergyExplosion(const CSkillSlot_EnergyExplosion & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_EnergyExplosion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_EnergyExplosion::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 3.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_EnergyExplosion::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_EnergyExplosion::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_EnergyExplosion * CSkillSlot_EnergyExplosion::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CSkillSlot_EnergyExplosion* pInstance = new CSkillSlot_EnergyExplosion(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CEnergyExploitationSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_EnergyExplosion::Clone_GameObject(void * _pArg)
{
	CSkillSlot_EnergyExplosion* pInstance = new CSkillSlot_EnergyExplosion(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CEnergyExploitationSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_EnergyExplosion::Actual_UseSkill(void* _pArg)
{
	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	//--------------------------------------------------
	// TODO : 폭발 소환
	//--------------------------------------------------

	--m_iCanUseCnt;
	return true;
}
