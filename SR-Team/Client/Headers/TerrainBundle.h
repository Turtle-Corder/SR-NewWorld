#pragma once

#ifndef __BUNDLE_TERRAIN_H__
#define __BUNDLE_TERRAIN_H__

USING(Engine)
BEGIN(Client)


class CCubeTerrain;
class CTerrainBundle final : public CGameObject
{
private:
	explicit CTerrainBundle(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTerrainBundle(const CTerrainBundle& _rOther);
	virtual ~CTerrainBundle() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;


	//Get 높이 길이 인터벌 타일인포
	//Set

	//Index에 대한 TerrainInfo를 강제로 지정해서 얻음
	TERRAININFO Get_TerrainInfo(_uint iIndex, _uint iFloor);

	//자기가 서있는 위치에 대한 Index를 계산하고 해당 Info를 얻어감.
	TERRAININFO Get_TerrainInfo(_vec3 iObjectPos, _uint iFloor);
	CGameObject* ApproachToCubeObject(_vec3 iObjectPos, _uint iFloor);
	//로딩중 세팅용
	HRESULT Set_TerrainInfo(_uint iIndex, _uint iFloor, const TERRAININFO& Input );

private:
	HRESULT Add_Component();


public:
	static CTerrainBundle* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;


private:
	//TerrainCube 인포
	vector<TERRAININFO> m_TerrianList[16];

};

END

#endif

