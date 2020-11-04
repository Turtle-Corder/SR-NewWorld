#pragma once
#ifndef __SCENE_STAGE0_H__
#define __SCENE_STAGE0_H__

#include "Scene.h"

BEGIN(Client)
USING(Engine)

class CPreLoader;
class CScene_Stage0 final : public CScene
{
private:
	explicit CScene_Stage0(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CScene_Stage0() = default;

public:
	virtual HRESULT Setup_Scene() override;
	virtual _int Update_Scene(_float _fDeltaTime) override;
	virtual _int LateUpdate_Scene(_float _fDeltaTime) override;

public:
	static CScene_Stage0* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;

private:
	HRESULT Setup_Layer_AllObject();


	HRESULT Setup_Layer_Skybox(const wstring& LayerTag);
	HRESULT Setup_Layer_Terrain(const wstring& LayerTag);
	HRESULT Setup_Layer_CubeTerrain(const wstring& LayerTag);
	HRESULT Setup_Layer_Environment(const wstring& LayerTag);
	HRESULT Setup_Layer_FlowerQuest(const wstring& LayerTag);

	
private:
	CPreLoader*		m_pPreLoader = nullptr;
	_bool			m_bReload = false;
	eSCENE_ID		m_ePreLoadSceneID = SCENE_END;
};

END

#endif
