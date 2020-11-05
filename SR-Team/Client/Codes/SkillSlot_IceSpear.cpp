#include "stdafx.h"
#include "..\Headers\SkillSlot_IceSpear.h"

USING(Client)

CSkillSlot_IceSpear::CSkillSlot_IceSpear(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_IceSpear::CSkillSlot_IceSpear(const CSkillSlot_IceSpear & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_IceSpear::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_IceSpear::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 10.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_IceSpear::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_IceSpear::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_IceSpear * CSkillSlot_IceSpear::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkillSlot_IceSpear* pInstance = new CSkillSlot_IceSpear(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSkillSlot_IceSpear", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_IceSpear::Clone_GameObject(void * _pArg)
{
	CSkillSlot_IceSpear* pInstance = new CSkillSlot_IceSpear(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkillSlot_IceSpear", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_IceSpear::Actual_UseSkill(void * _pArg)
{
	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	if (_pArg)
	{
		INSTANTIMPACT* pImpact = (INSTANTIMPACT*)_pArg;
		CStatus* pStatus = (CStatus*)pImpact->pStatusComp;
		if (pStatus)
			pStatus->Increase_IceStack();
	}

	//--------------------------------------------------
	// TODO 
	//--------------------------------------------------


	--m_iCanUseCnt;
	return true;
}
