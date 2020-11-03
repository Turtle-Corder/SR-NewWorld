#include "Timer.h"
#include "..\Headers\Timer_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

HRESULT CTimer_Manager::Setup_TimerManager()
{
	m_pTimers = new TIMERS;
	return S_OK;
}

_int CTimer_Manager::Update_TimeManager()
{
	for (auto& rPair : *m_pTimers)
	{
		if (FAILED(rPair.second->Update_Timer()))
			return -1;
	}

	return 0;
}

HRESULT CTimer_Manager::Add_Timer(const wstring & _strTimerTag, bool _bStart)
{
	auto iter_find = m_pTimers->find(_strTimerTag);
	if (m_pTimers->end() != iter_find)
	{

		PRINT_LOG(L"Add_Timer is Failed!", LOG::ENGINE);
		return E_FAIL;
	}

	CTimer *pTimer = nullptr;
	pTimer = CTimer::Create();
	if (nullptr == pTimer)
	{
		PRINT_LOG(L"Timer Create Failed!", LOG::ENGINE);
		return E_FAIL;
	}

	if (!_bStart) pTimer->Pause();
	m_pTimers->emplace(_strTimerTag, pTimer);
	return S_OK;
}

HRESULT CTimer_Manager::Get_DeltaTime(const wstring & _strTimerTag, float & _fDeltaTime)
{
	auto iter_find = m_pTimers->find(_strTimerTag);
	if (m_pTimers->end() == iter_find)
	{
		PRINT_LOG(L"Get_DeltaTime Cant Found Timer", LOG::ENGINE);
		return E_FAIL;
	}

	_fDeltaTime = iter_find->second->Get_DeltaTime();
	return S_OK;
}

HRESULT CTimer_Manager::Pause(const wstring & _strTimerTag)
{
	auto iter_find = m_pTimers->find(_strTimerTag);
	if (m_pTimers->end() == iter_find)
	{
		PRINT_LOG(L"Pause Cant Found Timer", LOG::ENGINE);
		return E_FAIL;
	}

	iter_find->second->Pause();
	return S_OK;
}

HRESULT CTimer_Manager::Resume(const wstring & _strTimerTag)
{
	auto iter_find = m_pTimers->find(_strTimerTag);
	if (m_pTimers->end() == iter_find)
	{
		PRINT_LOG(L"Resume Cant Found Timer", LOG::ENGINE);
		return E_FAIL;
	}

	iter_find->second->Resume();
	return S_OK;
}

void CTimer_Manager::Free()
{
	for (auto& rPair : *m_pTimers)
		Safe_Delete(rPair.second);

	Safe_Delete(m_pTimers);
}
