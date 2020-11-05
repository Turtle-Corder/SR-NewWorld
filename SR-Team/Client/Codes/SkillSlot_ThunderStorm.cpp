#include "stdafx.h"
#include "..\Headers\SkillSlot_ThunderStorm.h"

USING(Client)

CSkillSlot_ThunderStorm::CSkillSlot_ThunderStorm(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_ThunderStorm::CSkillSlot_ThunderStorm(const CSkillSlot_ThunderStorm & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_ThunderStorm::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_ThunderStorm::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 10.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_ThunderStorm::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_ThunderStorm::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_ThunderStorm * CSkillSlot_ThunderStorm::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkillSlot_ThunderStorm* pInstance = new CSkillSlot_ThunderStorm(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSkillSlot_ThunderStorm", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_ThunderStorm::Clone_GameObject(void * _pArg)
{
	CSkillSlot_ThunderStorm* pInstance = new CSkillSlot_ThunderStorm(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkillSlot_ThunderStorm", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_ThunderStorm::Actual_UseSkill(void * _pArg)
{
	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	if (_pArg)
	{
		INSTANTIMPACT* pImpact = (INSTANTIMPACT*)_pArg;
		CStatus* pStatus = (CStatus*)pImpact->pStatusComp;
	}

	//--------------------------------------------------
	// TODO :
	//--------------------------------------------------

	--m_iCanUseCnt;
	return true;
}
