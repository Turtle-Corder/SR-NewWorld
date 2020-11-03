#pragma once
#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#include "Base.h"

BEGIN(Client)
USING(Engine)

class CMainApp final : public CBase
{
private:
	CMainApp(); 
	virtual ~CMainApp() = default;

public:
	HRESULT Setup_MainApp();
	_int	Update_MainApp();
	HRESULT Render_MainApp();

	_bool Lock_FrameManager();
	void Render_FrameManager();

private:
	HRESULT Setup_DefaultSetting();
	HRESULT Setup_ProtoTypeData();

	HRESULT Setup_StaticResources();

	HRESULT Setup_HK();
	HRESULT Setup_YJ();
	HRESULT Setup_EB();
	HRESULT Setup_DS();

public:
	static CMainApp* Create();
	virtual void Free() override;
	
private:;
	CManagement*		m_pManagement	= nullptr;
	LPDIRECT3DDEVICE9	m_pDevice		= nullptr;
	LPD3DXSPRITE		m_pSprite		= nullptr;
	LPD3DXFONT			m_pFont			= nullptr;
};

END

#endif
