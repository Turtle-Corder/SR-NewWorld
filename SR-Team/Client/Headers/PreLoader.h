#pragma once
#ifndef __PRELOADER_H__
#define __PRELOADER_H__

#include "Base.h"

BEGIN(Client)
USING(Engine)

class CPreLoader : public CBase
{
private:
	explicit CPreLoader(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CPreLoader() = default;

public:
	_bool IsFinished();

public:
	HRESULT Setup_PreLoader(eSCENE_ID _eNextSceneID);

private:
	HRESULT Load_Resources_Room();
	HRESULT Load_Resources_Town();
	HRESULT Load_Resources_Forest();
	HRESULT Load_Resources_Iceland();
	HRESULT Load_Resources_Volcanic();
	HRESULT Load_Resources_Lab();

	HRESULT Setup_Stage_CubeTerrain(const wstring & LayerTag, const _uint & StageNumber);



private:
	static _uint WINAPI ThreadMain(void* _pParam);

public:
	static CPreLoader* Create(LPDIRECT3DDEVICE9 _pDevice, eSCENE_ID _eNextSceneID);
	virtual void Free() override;



private:
	_bool				m_bFinished = false;

	HANDLE				m_hLoadThread;
	CRITICAL_SECTION	m_CriticalSection;

	LPDIRECT3DDEVICE9	m_pDevice;
	eSCENE_ID			m_eNextSceneID;
};

END

#endif
