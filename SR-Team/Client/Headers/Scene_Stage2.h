#pragma once
#ifndef __SCENE_STAGE2_H__
#define __SCENE_STAGE2_H__

#include "Scene.h"

BEGIN(Client)
USING(Engine)

class CPreLoader;
class CScene_Stage2 final : public CScene
{
private:
	explicit CScene_Stage2(LPDIRECT3DDEVICE9 _pDeivce);
	virtual ~CScene_Stage2() = default;

public:
	virtual HRESULT Setup_Scene() override;
	virtual _int Update_Scene(_float _fDeltaTime) override;
	virtual _int LateUpdate_Scene(_float _fDeltaTime) override;

public:
	static CScene_Stage2* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;

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

	HRESULT Setup_Layer_NPC(const wstring& LayerTag);
	HRESULT SetUp_Layer_IceLandQuest(const wstring& LayerTag);

	HRESULT Travel_NextLayers();


private:
	CPreLoader* m_pPreLoader = nullptr;

	_bool		m_bInit = false;
};

END

#endif // !__SCENE_STAGE2_H__
