#pragma once
#ifndef __TIMER_MANAGER_H__
#define __TIMER_MANAGER_H__

#include "Base.h"

BEGIN(Engine)
class CTimer;
class CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)

private:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	HRESULT Setup_TimerManager();
	_int	Update_TimeManager();

	HRESULT Add_Timer(const wstring& _strTimerTag, bool _bStart);
	HRESULT Get_DeltaTime(const wstring& _strTimerTag, float& _fDeltaTime);

	HRESULT Pause(const wstring& _strTimerTag);
	HRESULT Resume(const wstring& _strTimerTag);

public:
	virtual void Free() override;



private:
	typedef unordered_map<wstring, CTimer*> TIMERS;
	TIMERS* m_pTimers = nullptr;
};

END

#endif
