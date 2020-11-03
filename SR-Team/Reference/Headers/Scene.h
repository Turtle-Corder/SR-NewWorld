#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CScene() = default;

public:
	virtual HRESULT Setup_Scene() = 0;
	virtual _int Update_Scene(_float _fDeltaTime) = 0;
	virtual _int LateUpdate_Scene(_float _fDeltaTime) = 0;

public:
	virtual void Free(void) override;


protected:
	LPDIRECT3DDEVICE9 m_pDevice;

};

END

#endif

