#include "Management.h"
#include "Component.h"
#include "..\Headers\UIObject.h"

USING(Engine)

CUIObject::CUIObject(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CGameObject(_pDevice)
	, m_pSprite(_pSprite)
	, m_pFont(_pFont)
{
	Safe_AddRef(m_pSprite);
	Safe_AddRef(m_pFont);
}

CUIObject::CUIObject(const CUIObject & _rOther)
	: CGameObject(_rOther)
	, m_pSprite(_rOther.m_pSprite)
	, m_pFont(_rOther.m_pFont)
{
	Safe_AddRef(m_pSprite);
	Safe_AddRef(m_pFont);
}

void CUIObject::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	CGameObject::Free();
}
