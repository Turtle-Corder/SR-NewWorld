#include "stdafx.h"
#include "..\Headers\SkillSlot_Blind.h"

USING(Client)

CSkillSlot_Blind::CSkillSlot_Blind(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_Blind::CSkillSlot_Blind(const CSkillSlot_Blind & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_Blind::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_Blind::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 10.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_Blind::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_Blind::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_Blind * CSkillSlot_Blind::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkillSlot_Blind* pInstance = new CSkillSlot_Blind(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMeteoSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_Blind::Clone_GameObject(void * _pArg)
{
	CSkillSlot_Blind* pInstance = new CSkillSlot_Blind(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMeteoSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_Blind::Actual_UseSkill(void * _pArg)
{
	INSTANTIMPACT* pImpact = nullptr;
	CTransform* pTargetTransform = nullptr;
	CStatus* pStatus = nullptr;

	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	if (_pArg)
	{
		pImpact = (INSTANTIMPACT*)_pArg;
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Blind", pManagement->Get_CurrentSceneID(), L"Layer_Effect", pImpact)))
	{
		PRINT_LOG(L"Failed To Spawn Blind", LOG::DEBUG);
		return false;
	}

	--m_iCanUseCnt;
	return true;
}
