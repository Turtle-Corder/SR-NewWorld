#include "stdafx.h"
#include "..\Headers\SkillSlot_Meteor.h"

USING(Client)

CSkillSlot_Meteor::CSkillSlot_Meteor(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_Meteor::CSkillSlot_Meteor(const CSkillSlot_Meteor & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_Meteor::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_Meteor::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 10.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_Meteor::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_Meteor::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_Meteor * CSkillSlot_Meteor::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkillSlot_Meteor* pInstance = new CSkillSlot_Meteor(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMeteoSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_Meteor::Clone_GameObject(void * _pArg)
{
	CSkillSlot_Meteor* pInstance = new CSkillSlot_Meteor(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMeteoSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_Meteor::Actual_UseSkill(void* _pArg)
{
	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	if (_pArg)
	{
		INSTANTIMPACT* pImpact = (INSTANTIMPACT*)_pArg;
		CStatus* pStatus = (CStatus*)pImpact->pStatusComp;
		if (pStatus)
			pStatus->Increase_FireStack();
	}

	//--------------------------------------------------
	// TODO : 메테오 소환, 5개 한방에 소환
	//--------------------------------------------------
	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{

	}
	
	--m_iCanUseCnt;
	return true;
}