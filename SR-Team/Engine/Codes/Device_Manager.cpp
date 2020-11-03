#include "..\Headers\Device_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CDevice_Manager)

CDevice_Manager::CDevice_Manager()
{
}

LPDIRECT3DDEVICE9 CDevice_Manager::Get_Device() const
{
	return m_pDevice;
}

LPD3DXFONT CDevice_Manager::Get_Font() const
{
	return m_pFont;
}

LPD3DXSPRITE CDevice_Manager::Get_Sprite() const
{
	return m_pSprite;
}

HRESULT CDevice_Manager::Setup_GraphicDevice(HWND _hWnd, _uint _iWinCX, _uint _iWinCY, DISPLAY_MODE _eDisplayMode)
{
	if (nullptr == _hWnd)
		return E_FAIL;


	D3DCAPS9 DeviceCap;
	ZeroMemory(&DeviceCap, sizeof(D3DCAPS9));


	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCap)))
	{
		PRINT_LOG(L"Failed To Create SDK", LOG::SYSTEM);
		return E_FAIL;
	}


	DWORD vp = 0;
	if (DeviceCap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;


	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferWidth	= _iWinCX;
	d3dpp.BackBufferHeight	= _iWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = _hWnd;
	d3dpp.Windowed = _eDisplayMode;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	//--------------------------------------------------
	// SDK
	//--------------------------------------------------
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd, vp, &d3dpp, &m_pDevice)))
	{
		PRINT_LOG(L"Failed To Create Device", LOG::SYSTEM);
		return E_FAIL;
	}

	//--------------------------------------------------
	// SPRITE
	//--------------------------------------------------
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		PRINT_LOG(L"Failed To Create Sprite", LOG::SYSTEM);
		return E_FAIL;
	}


	//--------------------------------------------------
	// FONT
	//--------------------------------------------------	
	D3DXFONT_DESC tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));
	tFontInfo.Height = 8;
	tFontInfo.Width = 8;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"±Ã¼­");
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		PRINT_LOG(L"Failed To Create Font", LOG::SYSTEM);
		return E_FAIL;
	}

	return S_OK;
}

void CDevice_Manager::Free()
{
	if (Safe_Release(m_pFont))
		PRINT_LOG(L"Failed To Release Font", LOG::SYSTEM);

	if(Safe_Release(m_pSprite))
		PRINT_LOG(L"Failed To Release Sprite", LOG::SYSTEM);

	if (Safe_Release(m_pDevice))
		PRINT_LOG(L"Failed To Release Device", LOG::SYSTEM);
	
	if (Safe_Release(m_pSDK))
		PRINT_LOG(L"Failed To Release SDK", LOG::SYSTEM);
}
