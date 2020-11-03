#pragma once
#ifndef __SCENE_ROOM_H__
#define __SCENE_ROOM_H__

#include "Scene.h"

USING(Engine)
BEGIN(Client)

class CPreLoader;
class CScene_Room final : public CScene
{
private:
	explicit CScene_Room(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CScene_Room() = default;

public:
	virtual HRESULT Setup_Scene() override;
	virtual _int Update_Scene(_float _fDeltaTime) override;
	virtual _int LateUpdate_Scene(_float _fDeltaTime) override;

public:
	static CScene_Room* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free() override;

private:
	HRESULT Setup_Layer_AllObject();

	HRESULT Setup_Layer_Skybox(const wstring& LayerTag);
	HRESULT Setup_Layer_Terrain(const wstring& LayerTag);
	HRESULT Setup_Layer_CubeTerrain(const wstring& LayerTag);
	HRESULT Setup_Layer_Environment(const wstring& LayerTag);

	HRESULT Setup_Layer_Mouse(const wstring& LayerTag);
	HRESULT Setup_Layer_Camera(const wstring& LayerTag);
	HRESULT Setup_Layer_Player(const wstring& LayerTag);

	HRESULT Setup_Layer_UI(const wstring& LayerTag);
	HRESULT SetUp_Layer_Inventory(const wstring& LayerTag);
	HRESULT SetUp_Layer_Shop(const wstring& LayerTag);
	HRESULT SetUp_Layer_Item(const wstring& LayerTag);

private:
	CPreLoader*		m_pPreLoader = nullptr;
};

END

#endif // !__SCENE_ROOM_H__
