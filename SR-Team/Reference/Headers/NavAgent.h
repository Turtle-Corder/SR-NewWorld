#pragma once
#ifndef __NAV_AGENT_H__
#define __NAV_AGENT_H__

#include "Component.h"

BEGIN(Engine)
class CGameObject;
class CNavAgent final : public CComponent
{
public:
	typedef struct tagNavDesc
	{
		CGameObject* pOwner = nullptr;
		_ivec2	tStart;
		_ivec2	tGoal;
	} NAV_DESC;

private:
	explicit CNavAgent(LPDIRECT3DDEVICE9 _pDevice);
	explicit CNavAgent(const CNavAgent& _rOther);
	virtual ~CNavAgent() = default;

public:
	bool IsFound() const;
	CGameObject* Get_Owner();
	_int Get_BranchSize();
	const NAV_DESC& Get_NavDesc() const;
	bool Get_NextGoal(_ivec2* _pBranch);

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void * _pArg) override;

	void Set_NavDone();
	void Set_NavDesc(const NAV_DESC& _tNavDesc);
	void Set_NavDesc(_int _iStartX, _int _iStartZ, _int _iGoalX, _int _iGoalZ);
	void Set_NavDesc(const _ivec2& _rStart, const _ivec2& _rGoal);	
	void Add_Branch(const _ivec2& _rBranch);
	void Add_Branch(_int _iX, _int _iZ);

public:
	static CNavAgent* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent * Clone_Component(void * _pArg) override;
	virtual void Free() override;



private:
	_bool			m_bNavDone = false;
	NAV_DESC		m_tNavDesc;
	list<_ivec2>	m_BranchList;
};

END

#endif // !__NAV_AGENT_H__
