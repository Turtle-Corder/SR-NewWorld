#pragma once
#ifndef __DAMAGEINFO_H__
#define __DAMAGEINFO_H__

#include "Component.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo final : public CComponent
{
public:
	typedef struct tagDamageDesc
	{
		CGameObject*	pOwner = nullptr;
		_int			iMinAtt = 0;
		_int			iMaxAtt = 0;
		_int			iCriticalRate = 0;
		_int			iCriticalHit = 0;
		eELEMENTAL_TYPE	eType = eELEMENTAL_TYPE::NONE;
	} DAMAGE_DESC;


private:
	explicit CDamageInfo(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDamageInfo(const CDamageInfo& _rOther);
	virtual ~CDamageInfo() = default;

public:
	const DAMAGE_DESC& Get_Desc() const;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

public:
	static CDamageInfo* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free(void) override;



private:
	DAMAGE_DESC m_tDamageDesc;
};

END

#endif // !__DAMAGEINFO_H__
