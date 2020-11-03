#pragma once
#ifndef __FRAME_MANAGER_H__
#define __FRAME_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

class CFrame_Manager final : public CBase
{
	DECLARE_SINGLETON(CFrame_Manager)

private:
	CFrame_Manager();
	virtual ~CFrame_Manager() = default;

public:
	HRESULT Setup_FrameManager(HWND _hWnd, _uint _iFramePerSec);

	_bool Lock_FrameManager();

	void Render_FrameManager();

public:
	virtual void Free() override;



private:
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_PrevTime;
	LARGE_INTEGER m_CurrTime;

	// 1회 update 하기 위해 필요한 딜레이 시간
	double m_dSecondPerFrame;

	// 이전 업데이트 성공과 현재 업데이트 성공 사이의 시간
	double m_dDeltaTime;

	// 1회 update 하기 위해 흐른 시간을 누적
	double m_dElapseTime;


	//--------------------------------------------------
	// 출력용
	//--------------------------------------------------
	HWND	m_hWnd = NULL;
	int		m_iFPS;
	TCHAR	m_szFPS[32];

	// 1초를 확인 하기 위한 변수
	double m_dSecElapseTime;

};

END

#endif // !__FRAME_MANAGER_H__
