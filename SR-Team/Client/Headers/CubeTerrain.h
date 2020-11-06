#pragma once
#ifndef __CUBE_TERRAIN_H__
#define __CUBE_TERRAIN_H__

USING(Engine)
BEGIN(Client)

class CCubeTerrain final : public CGameObject
{
private:
	explicit CCubeTerrain(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCubeTerrain(const CCubeTerrain& _rOther);
	virtual ~CCubeTerrain() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();


public:
	static CCubeTerrain* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;


private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;

	//TerrainCube ÀÎÆ÷
	TERRAININFO m_tInfo;
};

END

#endif // !__CUBE_TERRAIN_H__
