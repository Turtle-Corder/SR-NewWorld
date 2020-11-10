#pragma once
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Base.h"

BEGIN(Engine)

class CScene;
class CScene_Manager final : public CBase
{
	DECLARE_SINGLETON(CScene_Manager)

private:
	CScene_Manager();
	virtual ~CScene_Manager() = default;

public:
	_int Get_CurrentSceneID();
	CScene* GetCurrentScene();

public:
	HRESULT Change_CurrentScene(_int _iSceneID, CScene* pCurrentScene);
	_int Update_Scene_Manager(_float _fDeltaTime);
	_int LateUpdate_Scene_Manager(_float _fDeltaTime);


public:
	virtual void Free() override;



private:
	CScene*		m_pCurrentScene		= nullptr;
	_int		m_iCurrentSceneID	= -1;
};

END

#endif
