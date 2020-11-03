#pragma once
#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

class CDevice_Manager final : public CBase
{
	//----------------------------------------------------------------------------------------------------
	// 함수
	//----------------------------------------------------------------------------------------------------
	DECLARE_SINGLETON(CDevice_Manager)

private:
	CDevice_Manager();
	virtual ~CDevice_Manager() = default;

public:
	enum DISPLAY_MODE
	{
		DISPLAY_FULL,
		DISPLAY_WINDOW
	};

public:
	LPDIRECT3DDEVICE9	Get_Device()	const;
	LPD3DXFONT			Get_Font()		const;
	LPD3DXSPRITE		Get_Sprite()	const;


	HRESULT Setup_GraphicDevice(HWND _hWnd, _uint _iWinCX, _uint _iWinCY, DISPLAY_MODE _eDisplayMode);

	virtual void Free() override;



	//----------------------------------------------------------------------------------------------------
	// 변수
	//----------------------------------------------------------------------------------------------------
private:
	LPDIRECT3D9			m_pSDK		= nullptr;
	LPDIRECT3DDEVICE9	m_pDevice	= nullptr;

	LPD3DXSPRITE		m_pSprite	= nullptr;
	LPD3DXFONT			m_pFont		= nullptr;
};

END

#endif
