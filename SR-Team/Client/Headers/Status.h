#pragma once
#ifndef __STATUS_H__
#define __STATUS_H__

#include "Component.h"

USING(Engine)
BEGIN(Client)

class CStatus final : public CComponent
{
public:
	typedef struct tagStat
	{
		_int	iMinAtt = 0;
		_int	iMaxAtt = 0;
		_int	iCriticalRate = 0;
		_int	iCriticalHit = 0;
		_int	iDef = 0;

		_int	iMaxHp = 0;
		_int	iMaxMp = 0;
		_int	iHp = 0;
		_int	iMp = 0;
		_int	iSort = 0;

		_int	iLevel = 0;
		_int	iExp = 0;
		_int	iMaxExp = 0;

		_int	iCurFireStack = 0;
		_int	iMaxFireStack = 0;
		
		_int	iCurIceStack = 0;
		_int	iMaxIceStack = 0;
	} STAT;

private:
	explicit CStatus(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStatus(const CStatus& _rOther);
	virtual ~CStatus() = default;

public:
	const STAT& Get_Status() const;

public:
	void Set_Status(const STAT& _rStatus);
	void Set_Att(_int iAtt);
	void Set_HP(_int _iAtt);
	void Set_MP(_int _iMP);
	void Change_Hp(_int iHp) { m_tStat.iHp = iHp; }
	void Change_Mp(_int iMp) { m_tStat.iMp = iMp; }

	// 속성 스택 카운트를 하나 올린 결과를 반환한다.
	_int Increase_FireStack();
	_int Increase_IceStack();

	// 속성 스택을 초기화 한다.
	void Clear_FireStack();
	void Clear_IceStack();

	// 착용하고 있는 장비의 스탯으로 설정한다
	void Set_EquipStat(const CStatus::STAT& tStat);


public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

public:
	static CStatus* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent * Clone_Component(void * _pArg) override;
	virtual void Free() override;
public:
	const STAT& Get_Status() { return m_tStat; }


private:
	STAT m_tStat;
};

END

#endif
