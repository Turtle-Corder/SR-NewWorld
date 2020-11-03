#pragma once
#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__

#include "Scene.h"

BEGIN(Client)
USING(Engine)

class CPreLoader;
class CScene_Logo final : public CScene
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CScene_Logo() = default;

public:
	virtual HRESULT Setup_Scene() override;
	virtual _int Update_Scene(_float _fDeltaTime) override;
	virtual _int LateUpdate_Scene(_float _fDeltaTime) override;

public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;



private:
	CPreLoader*		m_pPreLoader = nullptr;
};

END

#endif
