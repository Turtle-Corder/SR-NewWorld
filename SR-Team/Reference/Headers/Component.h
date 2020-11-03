#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 _pDevice);
	explicit CComponent(const CComponent& _rOther);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Setup_Component_Prototype() = 0;
	virtual HRESULT Setup_Component(void* _pArg) = 0;

public:
	virtual CComponent* Clone_Component(void* _pArg) = 0;
	virtual void Free(void) override;



protected:
	LPDIRECT3DDEVICE9 m_pDevice;
};

END

#endif

