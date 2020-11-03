#pragma once
#ifndef __UIOBJECT_H__
#define __UIOBJECT_H__

#include "Base.h"

BEGIN(Engine)
class CComponent;
class ENGINE_DLL CUIObject abstract : public CGameObject
{
protected:
	explicit CUIObject(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CUIObject(const CUIObject& _rOther);
	virtual ~CUIObject() = default;

public:
	virtual void Free() override;


protected:
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
};

END

#endif // !__UIOBJECT_H__
