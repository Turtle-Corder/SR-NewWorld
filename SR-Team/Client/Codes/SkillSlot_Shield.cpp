#include "stdafx.h"
#include "Shield.h"
#include "Player.h"
#include "..\Headers\SkillSlot_Shield.h"

USING(Client)

CSkillSlot_Shield::CSkillSlot_Shield(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_Shield::CSkillSlot_Shield(const CSkillSlot_Shield & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_Shield::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_Shield::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 30.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_Shield::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_Shield::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_Shield * CSkillSlot_Shield::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkillSlot_Shield* pInstance = new CSkillSlot_Shield(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMeteoSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_Shield::Clone_GameObject(void * _pArg)
{
	CSkillSlot_Shield* pInstance = new CSkillSlot_Shield(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMeteoSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_Shield::Actual_UseSkill(void * _pArg)
{
	INSTANTIMPACT* pImpact = nullptr;
	CStatus* pStatus = nullptr;

	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	if (_pArg)
	{
		pImpact = (INSTANTIMPACT*)_pArg;

		CPlayer* pPlayer = (CPlayer*)pImpact->pAttacker;
		if (!pPlayer)	return false;

		if (!pPlayer->IsOnBuff(CPlayer::BUFF_SHIELD))
		{
			pPlayer->Buff_On(CPlayer::BUFF_SHIELD);
		}
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;
	
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Shield", pManagement->Get_CurrentSceneID(), L"Layer_Effect", pImpact)))
	{
		PRINT_LOG(L"Failed To Spawn Meteor", LOG::DEBUG);
		return false;
	}


	--m_iCanUseCnt;
	return true;
}
