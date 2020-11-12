#include "stdafx.h"
#include "Player.h"
#include "..\Headers\SkillSlot_IceBlast.h"

USING(Client)

CSkillSlot_IceBlast::CSkillSlot_IceBlast(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CSkillSlot(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot_IceBlast::CSkillSlot_IceBlast(const CSkillSlot_IceBlast & _rOther)
	: CSkillSlot(_rOther)
{
}

HRESULT CSkillSlot_IceBlast::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot_IceBlast::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 5.f;

	m_iConsumeMP = 20;

	return S_OK;
}

_int CSkillSlot_IceBlast::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillSlot_IceBlast::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CSkillSlot_IceBlast * CSkillSlot_IceBlast::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkillSlot_IceBlast* pInstance = new CSkillSlot_IceBlast(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSkillSlot_IceBlast", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillSlot_IceBlast::Clone_GameObject(void * _pArg)
{
	CSkillSlot_IceBlast* pInstance = new CSkillSlot_IceBlast(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkillSlot_IceBlast", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CSkillSlot_IceBlast::Actual_UseSkill(void * _pArg)
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
			iStackCnt = pStatus->Get_Status().iCurIceStack;
			if (iStackCnt <= 0)
				return false;

			CPlayer* pPlayer = (CPlayer*)pImpact->pAttacker;
			if (!pPlayer)
				return false;

			pPlayer->DeActive_IceCrystal();

			pStatus->Clear_IceStack();
		}
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	for (_uint iCnt = 0; iCnt < iStackCnt * 50; ++iCnt)
	{
		if (pImpact)
		{
			pImpact->vOption.x = (_float)iCnt;
			pImpact->vOption.y = (_float)iStackCnt * 2.f;
		}

		_vec3 vCircleVector = { cos((_float)iCnt * 0.8f), 0.f, sin((_float)iCnt) * 0.8f };
		pImpact->vPosition += vCircleVector;

		if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_IceBlast", pManagement->Get_CurrentSceneID(), L"Layer_PlayerAtk", pImpact)))
		{
			PRINT_LOG(L"Failed To Spawn IceBlast", LOG::DEBUG);
			return false;
		}
	}

	--m_iCanUseCnt;
	return true;
}
