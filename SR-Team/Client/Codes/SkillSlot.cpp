#include "stdafx.h"
#include "..\Headers\SkillSlot.h"


USING(Client)


CSkillSlot::CSkillSlot(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
}

CSkillSlot::CSkillSlot(const CSkillSlot & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CSkillSlot::Update_Delay(_float _fDeltaTime)
{
	if (m_iCanUseCnt < m_iMaxUseCnt)
	{
		m_fDelayCnt += _fDeltaTime;
		if (m_fDelayCnt > m_fEachDelay)
		{
			++m_iCanUseCnt;
			m_fDelayCnt = 0.f;
		}
	}

	return S_OK;
}

_float CSkillSlot::Get_Progress()
{
	return m_fDelayCnt / m_fEachDelay;
}

_bool CSkillSlot::Can_UseSkill()
{
	if (0 >= m_iCanUseCnt)
		return false;

	return true;
}

_bool CSkillSlot::Actual_UseSkill(void* _pArg)
{
	return false;
}

_int CSkillSlot::Get_ConsumeMP()
{
	return m_iConsumeMP;
}

void CSkillSlot::Free()
{
	CUIObject::Free();
}
