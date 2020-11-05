#include "stdafx.h"
#include "..\Headers\SkillSlot_Explosion.h"


USING(Client)

CSkillSlot_Explosion::CSkillSlot_Explosion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_Explosion::CSkillSlot_Explosion(const CSkillSlot_Explosion & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_Explosion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_Explosion::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 3.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_Explosion::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_Explosion::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_Explosion * CSkillSlot_Explosion::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CSkillSlot_Explosion* pInstance = new CSkillSlot_Explosion(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CEnergyExploitationSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_Explosion::Clone_GameObject(void * _pArg)
{
	CSkillSlot_Explosion* pInstance = new CSkillSlot_Explosion(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CEnergyExploitationSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_Explosion::Actual_UseSkill(void* _pArg)
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
