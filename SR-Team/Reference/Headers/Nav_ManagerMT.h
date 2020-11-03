#pragma once
#ifndef __NAV_MANAGER_MT_H__
#define __NAV_MANAGER_MT_H__

#include "Base.h"

BEGIN(Engine)
class CNavAgent;
class CNavigation_ManagerMT final : public CBase
{
	DECLARE_SINGLETON(CNavigation_ManagerMT)

private:
	CNavigation_ManagerMT();
	virtual ~CNavigation_ManagerMT() = default;

public:
	//--------------------------------------------------
	// 해당 함수는 메인 스레드 update 도중에 호출되므로 object가 무조건 살아 있다.
	// gameobject로 받으면 Done에 Enq시 NavAgent Tag 검색이 어렵다.
	//--------------------------------------------------
	HRESULT Add_NavAgent(CNavAgent* _pNavAgent);

	HRESULT Setup_NavigationManager();
	_int	Update_NavgationManager();

private:
	HRESULT NavUpdateThread();
	static _uint WINAPI Call_NavUpdateThread(LPVOID _lpParam);

public:
	virtual void Free() override;


private:
	HANDLE	m_hNavThread	= NULL;
	HANDLE	m_hNavEvent		= NULL;
	bool	m_bThreadDown	= false;
	_int	m_iCntPerCycle	= 0;
	_int	m_iMaxPerCycle	= 10;

	queue<CNavAgent*>	m_QueueWait;
	queue<CNavAgent*>	m_QueueDone;
};

END

#endif // !__NAV_MANAGER_MT_H__
