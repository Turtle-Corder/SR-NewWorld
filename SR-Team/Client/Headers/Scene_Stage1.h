#pragma once
#ifndef __SCENE_STAGE1_H__
#define __SCENE_STAGE1_H__

#include "Scene.h"

BEGIN(Client)
USING(Engine)

class CPreLoader;
class CScene_Stage1 final : public CScene
{
private:
	explicit CScene_Stage1(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CScene_Stage1() = default;

public:
	virtual HRESULT Setup_Scene() override;
	virtual _int Update_Scene(_float _fDeltaTime) override;
	virtual _int LateUpdate_Scene(_float _fDeltaTime) override;
	virtual HRESULT Set_SceneEvent(_int _iEventNo) override;

public:
	static CScene_Stage1* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;

private:
	HRESULT Setup_Layer_AllObject();

	//--------------------------------------------------
	// Mouse, Camera, Player, UI는 Layer통째로 복사해 올 것임
	//--------------------------------------------------
	HRESULT Setup_Layer_Skybox(const wstring& LayerTag);
	HRESULT Setup_Layer_Terrain(const wstring& LayerTag);
	HRESULT Setup_Layer_CubeTerrain(const wstring& LayerTag);
	HRESULT Setup_Layer_Environment(const wstring& LayerTag);

	HRESULT Setup_Layer_Monster(const wstring& LayerTag);
	HRESULT Setup_Layer_Player_Attack(const wstring& LayerTag);
	HRESULT Setup_Layer_Monster_Attack(const wstring& LayerTag);

	HRESULT Setup_Layer_Projectile();
	HRESULT Setup_Layer_ActiveObject(const wstring& LayerTag);

	HRESULT Setup_Layer_NPC(const wstring& LayerTag);

	HRESULT Travel_NextLayers();

	HRESULT Respawn_Palyer();

private:
	CPreLoader* m_pPreLoader = nullptr;

	_bool		m_bInit = false;

	_bool		m_bBossInit = false;
	_bool		m_bClear = false;
	_bool		m_bTravel = false;

	CGameObject*	m_pTravelTrigger = nullptr;

};

END

#endif // DEBUG
