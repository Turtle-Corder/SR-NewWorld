#include "stdafx.h"
#include "Player.h"
#include "MainCamera.h"
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
		PRINT_LOG(L"Failed To Create CSkillSlot_Meteor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_Meteor::Clone_GameObject(void * _pArg)
{
	CSkillSlot_Meteor* pInstance = new CSkillSlot_Meteor(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkillSlot_Meteor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_Meteor::Actual_UseSkill(void* _pArg)
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

		if (pStatus)
			pStatus->Increase_FireStack();

		CPlayer* pPlayer = (CPlayer*)pImpact->pAttacker;
		if (pPlayer)
			pPlayer->Active_FireCrystal();
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	//--------------------------------------------------
	// TODO : 메테오 소환, 5개 한방에 소환
	//--------------------------------------------------
	_vec3 vBaseSpawnPos = pImpact->vPosition;


	CMainCamera* pMainCamera = (CMainCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");

	pMainCamera->Set_Camera_Wigging(0.7f, 100.f, 1.5f, CMainCamera::WIG_TYPE::DAMPED);

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{
		//떨어지기 시작하는 위치
		_float fRandomness = 0.3f;
		_float fDirectionCorrention = 0.9f;
		_vec3 vAddSpawnPos = { (_float)(rand() % 60 - 30) * fRandomness, (_float)(rand() % 9 - 4) * 0.3f +14.f, (_float)(rand() % 60 - 30) * fRandomness };
		pImpact->vOption = vBaseSpawnPos;
		pImpact->vPosition = vBaseSpawnPos + vAddSpawnPos;
		pImpact->vDirection =  vBaseSpawnPos - pImpact->vPosition;
		pImpact->vDirection.x *= fDirectionCorrention;
		pImpact->vDirection.z *= fDirectionCorrention;

		if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Meteor", pManagement->Get_CurrentSceneID(), L"Layer_PlayerAtk", pImpact)))
		{
			PRINT_LOG(L"Failed To Spawn Meteor", LOG::DEBUG);
			return false;
		}
	}

	--m_iCanUseCnt;
	return true;
}