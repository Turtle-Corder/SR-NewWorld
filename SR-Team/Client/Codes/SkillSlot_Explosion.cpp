#include "stdafx.h"
#include "Player.h"
#include "..\Headers\SkillSlot_Explosion.h"
#include "Sound_Manager.h"

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

	m_iConsumeMP = 20;

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
	INSTANTIMPACT* pImpact = nullptr;
	CStatus* pStatus = nullptr;
	_uint iStackCnt = 0;

	if (!Can_UseSkill())
		return false;

	if (_pArg)
	{
		pImpact = (INSTANTIMPACT*)_pArg;
		pStatus = (CStatus*)pImpact->pStatusComp;
		if (pStatus)
		{
			iStackCnt = pStatus->Get_Status().iCurFireStack;
			if (iStackCnt <= 0)
				return false;

			CPlayer* pPlayer = (CPlayer*)pImpact->pAttacker;
			if (!pPlayer)
				return false;

			if (pPlayer->IsOnBuff(CPlayer::BUFF_ATTACK))
				return false;

			pPlayer->Buff_On(CPlayer::BUFF_ATTACK);
			pPlayer->DeActive_FireCrystal();
			pStatus->Clear_FireStack();
			pImpact->vOption.x = (_float)iStackCnt;
		}
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Explosion", pManagement->Get_CurrentSceneID(), L"Layer_Effect", pImpact)))
	{
		PRINT_LOG(L"Failed To Spawn IceBlast", LOG::DEBUG);
		return false;
	}

	CSoundManager::Get_Instance()->PlayEffect(L"BuffEffect.wav");

	--m_iCanUseCnt;
	return true;
}
