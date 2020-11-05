#include "stdafx.h"
#include "..\Headers\ItemSlot.h"


USING(Client)


CItemSlot::CItemSlot(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
}

CItemSlot::CItemSlot(const CItemSlot & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CItemSlot::Update_Delay(_float _fDeltaTime)
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

_bool CItemSlot::Can_UseItem()
{
	if (0 >= m_iCanUseCnt)
		return false;

	return true;
}

_bool CItemSlot::Actual_UseItem()
{
	return false;
}

_float CItemSlot::Get_Progress()
{
	return m_fDelayCnt / m_fEachDelay;
}

void CItemSlot::Free()
{
	CUIObject::Free();
}
