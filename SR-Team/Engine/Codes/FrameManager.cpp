#include "..\Headers\FrameManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CFrame_Manager)
CFrame_Manager::CFrame_Manager()
	: m_dSecondPerFrame(0.0), m_dDeltaTime(0.0), m_dElapseTime(0.0)
	, m_iFPS(0), m_szFPS(L""), m_dSecElapseTime(0.0)
{
}

HRESULT CFrame_Manager::Setup_FrameManager(HWND _hWnd, _uint _iFramePerSec)
{
	if (nullptr == _hWnd)
		return E_FAIL;

	m_hWnd = _hWnd;
	m_dSecondPerFrame = 1.0 / _iFramePerSec;

	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_PrevTime);
	QueryPerformanceCounter(&m_CurrTime);

	return S_OK;
}

_bool CFrame_Manager::Lock_FrameManager()
{
	// 시간 측정
	QueryPerformanceCounter(&m_CurrTime);

	// 얼마나 흘렀는지 시간을 구한다.
	double dCurrElapseTime = (double)(m_CurrTime.QuadPart - m_PrevTime.QuadPart) / m_Frequency.QuadPart;

	// 흐른 시간을 누적한다.
	m_dElapseTime += dCurrElapseTime;

	// 현재 시간을 이전 시간에 저장
	m_PrevTime = m_CurrTime;

	// 1회 Update 하기 위한 시간이 충족되었으면 
	if (m_dSecondPerFrame < m_dElapseTime)
	{
		// 1. 흐른 시간을 백업(저장)해두고,
		m_dDeltaTime = m_dElapseTime;

		// 2. 흐른 누적시간을 초기화 해준다.
		// 감소만 시키면 계속 밀림
		//m_dElapseTime -= m_dSecondPerFrame;
		m_dElapseTime = 0.0;

		// 업데이트 가능이라고 알려준다. return true
		return true;
	}

	return false;
}

void CFrame_Manager::Render_FrameManager()
{
	m_dSecElapseTime += m_dDeltaTime;
	++m_iFPS;
	if (m_dSecElapseTime > 1.0)
	{
		StringCchPrintf(m_szFPS, _countof(m_szFPS), L"FPS:%d", m_iFPS);
		m_dSecElapseTime -= 1.0;
		m_iFPS = 0;

		SetWindowText(m_hWnd, m_szFPS);
	}
}

void CFrame_Manager::Free()
{
}
