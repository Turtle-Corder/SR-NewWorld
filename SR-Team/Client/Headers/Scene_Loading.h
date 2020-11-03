#pragma once
#ifndef __SCENE_LOADING_H__
#define __SCENE_LOADING_H__

#include "Scene.h"

USING(Engine)
BEGIN(Client)

class CScene_Loading final : public CScene
{
private:
	explicit CScene_Loading(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CScene_Loading() = default;

public:
	virtual HRESULT Setup_Scene() override;
	virtual _int Update_Scene(_float _fDeltaTime) override;
	virtual _int LateUpdate_Scene(_float _fDeltaTime) override;

	static CScene_Loading* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;

private:
	_bool	m_bFinished = false;
	HANDLE	m_hLoadThread = NULL;
	CRITICAL_SECTION m_CriticalSection;

};

END

#endif
