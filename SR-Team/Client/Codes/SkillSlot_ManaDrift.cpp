#include "stdafx.h"
#include "Player.h"
#include "..\Headers\SkillSlot_ManaDrift.h"
#include "Equip.h"
#include "Sound_Manager.h"

USING(Client)


CSkillSlot_ManaDrift::CSkillSlot_ManaDrift(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_ManaDrift::CSkillSlot_ManaDrift(const CSkillSlot_ManaDrift & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_ManaDrift::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_ManaDrift::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 20.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CSkillSlot_ManaDrift::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_ManaDrift::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_ManaDrift * CSkillSlot_ManaDrift::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CSkillSlot_ManaDrift* pInstance = new CSkillSlot_ManaDrift(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CManaDriftSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_ManaDrift::Clone_GameObject(void * _pArg)
{
	CSkillSlot_ManaDrift* pInstance = new CSkillSlot_ManaDrift(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CManaDriftSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_ManaDrift::Actual_UseSkill(void* _pArg)
{
	INSTANTIMPACT* pImpact = nullptr;
	CStatus* pStatus = nullptr;

	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	if (_pArg)
	{
		pImpact = (INSTANTIMPACT*)_pArg;

		if (pImpact)
			pStatus = (CStatus*)pImpact->pStatusComp;

		CPlayer* pPlayer = (CPlayer*)pImpact->pAttacker;
		if (!pPlayer)	return false;

		if (pPlayer->IsOnBuff(CPlayer::BUFF_MANA))
			return false;

		pPlayer->Buff_On(CPlayer::BUFF_MANA);
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;


	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_MPDrift", pManagement->Get_CurrentSceneID(), L"Layer_PlayerAtk", pImpact)))
	{
		PRINT_LOG(L"Failed To Spawn Meteor", LOG::DEBUG);
		return false;
	}

	CSoundManager::Get_Instance()->PlayEffect(L"Buffing.wav");

	--m_iCanUseCnt;
	return true;
}
